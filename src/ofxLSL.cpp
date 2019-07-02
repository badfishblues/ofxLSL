#include "ofxLSL.h"

ofxLSL::ofxLSL() : active(false) {}

bool ofxLSL::start() {
	if(active) return false;
	thread = std::make_unique<std::thread>(&ofxLSL::update, this);
	active = true;
	return true;
}

bool ofxLSL::stop() {
	if(!active) return false;
	
	active = false;
	thread->join();
	thread = nullptr;
	return true;
}

void ofxLSL::update() {
	ofLogNotice() << "ofxLSL::update()";
	while(active) {
		try {
			if(inlet == nullptr) {
				ofLogNotice() << "ofxLSL::connect()";
				connect();
			} else {
				//ofLogNotice() << "Pulling";
				pullSamples();
				pullStability();
			}
		} catch(lsl::timeout_error& e) {
			disconnect();
		} catch(lsl::lost_error& e) {
			disconnect();
		}
	}
}

void ofxLSL::disconnect() {
	ofLogWarning() << "Disconnected from stream";
	
	std::lock_guard<std::mutex> lock(mutex);
	inlet = nullptr;
	buffer.clear();
	sample_buffer.clear();
	
	for (auto& i: stability_inlets) { i = nullptr; }
	stability_inlets.clear();
}

void ofxLSL::connect() {
	//auto streams = lsl::resolve_stream("desc/correlation", "R", 1, 2.f);
	auto streams = lsl::resolve_stream(resInletInfo);
	if (streams.size() == 0) {
		ofLogNotice() << "No Streams Found";
		return;
	}
	std::lock_guard<std::mutex> lock(mutex);
	inlet = std::make_unique<lsl::stream_inlet>(streams.front(), 360, 0, false);
	
	auto info = inlet->info(1.0f);
	starttime = lsl::local_clock();
	//ofLogNotice() << ofToString(starttime, 15) << '\t' << ofToString(inlet->time_correction(1), 15) << '\n';

	ofLogNotice() << "Connecting to " << info.name() << " at " << info.nominal_srate() << "hz";
	buffer.reserve(250.0);
	sample_buffer.reserve(info.channel_count());
	
	sample_mapping.clear();
	auto mappingXML = info.desc().child("mappings");
	for (auto m = mappingXML.first_child(); !m.empty(); m = m.next_sibling()) {
		bool firstSet = false;
		pair<string,string> mapElement;
		for (auto ab = m.first_child(); !ab.empty(); ab = ab.next_sibling()) {
			for (auto uID = ab.first_child(); !uID.empty(); uID = uID.next_sibling()) {
				if (!firstSet) {
					firstSet =true;
					mapElement.first = uID.value();
				} else {
					mapElement.second = uID.value();
					sample_mapping.emplace_back(mapElement);
				} } } }
	
	//auto Qstreams = lsl::resolve_stream("desc/correlation", "Stability", 1, 2.f);
	auto Qstreams = lsl::resolve_stream(resInletInfo);
	std::vector<lsl::stream_info> resolvedStreams = lsl::resolve_streams(1.0);
	
	stabilities.clear();
	for (auto &s : resolvedStreams) {
		string ns = s.name();
//		cout << ns << endl;
		if (ns.find(" ") < ns.length()) {
			string es = ns.substr(ns.find(" ") + 1, ns.length()-1);
			if (es.compare("Stability") == 0) {
				string cs = ns.substr(0, ns.find("-"));
//				cout << cs << " " << s.type() << endl;
				
				ofxStability stb;
				stb.color = cs;
				stb.sample.resize(4, 0);
				stabilities.emplace_back(stb);
				
				std::unique_ptr<lsl::stream_inlet> stability_inlet = std::make_unique<lsl::stream_inlet>(s, 360, 0, false);
				stability_inlets.push_back(std::move(stability_inlet));
			}
		}
	}
}

#if 0
void ofxLSL::pullSamples() {
	double ts = inlet->pull_sample(sample_buffer, 1.0);
	if(ts) {
		ofLogVerbose() << "Received sample";
		ofxLSLSample sample;
		sample.timestamp = ts;
		//changed to vector of strings
		sample.sample = std::vector<string>(sample_buffer.begin(), sample_buffer.end());
		
		std::lock_guard<std::mutex> lock(mutex);
		while(buffer.size() && buffer.size() >= buffer.capacity()) {
			ofLogWarning() << "Buffer capacity reached";
			buffer.erase(buffer.begin());
		}
		
		buffer.push_back(sample);
	}
}
#endif

void ofxLSL::pullSamples() {
	double ts = inlet->pull_sample(sample_buffer, 1.0);
	if (ts) {
		ofxLSLSample sample;
		sample.timestampLocal = ts + inlet->time_correction(1);
		sample.localClock = lsl::local_clock();
		sample.timestamp = ts;
		//sample.timestamp = ts - inlet->time_correction(1) - starttime;
		
		ofLogVerbose() << "Received sample";
		//changed to vector of strings
		sample.sample = std::vector<string>(sample_buffer.begin(), sample_buffer.end());

		std::lock_guard<std::mutex> lock(mutex);
		while (buffer.size() && buffer.size() >= buffer.capacity()) {
			ofLogWarning() << "Buffer capacity reached";
			buffer.erase(buffer.begin());
		}

		buffer.push_back(sample);
	}
}

void ofxLSL::pullStability() {
	
	for (int i=0; i<stability_inlets.size(); i++) {
		unique_ptr<lsl::stream_inlet>& stab_inlet = stability_inlets[i];
		
		std::vector<int> stability_buffer;
		float ts = stab_inlet->pull_sample(stability_buffer, 1.0);
		if(ts) {
			std::lock_guard<std::mutex> lock(mutex);
			stabilities[i].sample = stability_buffer;
		}
	}
}


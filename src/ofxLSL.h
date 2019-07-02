#pragma once

#include "ofMain.h"
#include "ofLog.h"
#include "lsl_cpp.h"

#if 0
struct ofxLSLSample {
	double timestamp = 0.0;
	//std::vector<float> sample;
	std::vector<string> sample;
};
#endif

struct ofxLSLSample {
	double timestamp = 0.0;
	double timestampLocal = 0.0;
	double localClock = 0.0;
	std::vector<string> sample;
};

struct ofxStability {
	string color = "none";
	std::vector<int> sample;
};

class ofxLSL : public ofThread {
	
public:
	static const char * resInletInfo;

	ofxLSL();
	~ofxLSL() { stop(); };
	
	bool start();
	bool stop();
	bool isConnected() {
		std::lock_guard<std::mutex> lock(mutex);
		return inlet != nullptr;
		
	}
	
	std::vector<ofxLSLSample> flush() {
		std::lock_guard<std::mutex> lock(mutex);
		auto currentBuffer = std::vector<ofxLSLSample>(buffer.begin(), buffer.end());
		buffer.clear();
		return currentBuffer;
	};
	
	std::vector< pair<string, string> > getMapping() {
		std::lock_guard<std::mutex> lock(mutex);
		return sample_mapping;
	};
	
	std::vector< ofxStability > getStability() {
		std::lock_guard<std::mutex> lock(mutex);
		return stabilities;
	};
	
	bool isActive() { return active; }
	bool getTimeCor() {
		std::lock_guard<std::mutex> lock(mutex);
		return inlet->time_correction(1);
	}
private:
	
	void update();
	void connect();
	void disconnect();
	void pullSamples();
	void pullStability();
	bool active;
	double starttime;
	//	std::vector<float> sample_buffer;
	std::vector<string> sample_buffer;
	std::vector< pair<string, string> > sample_mapping;
	
	std::vector< std::unique_ptr<lsl::stream_inlet> > stability_inlets;
	std::vector< ofxStability > stabilities;
	
	std::mutex mutex;
	std::unique_ptr<std::thread> thread;
	std::unique_ptr<lsl::stream_inlet> inlet;
	std::vector<ofxLSLSample> buffer;
};


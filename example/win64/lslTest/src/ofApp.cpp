#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofLogNotice() << "Starting";
	lsl.start();

}

//--------------------------------------------------------------
void ofApp::update(){
	ofLogLevel(OF_LOG_VERBOSE);
	//ofLogNotice() << "Updating";
//	if (lsl.isConnected()) {
		auto buffer = lsl.flush();
		
		if(buffer.size()) {
			auto sampleToUse = buffer.back();
			std::stringstream ss;
			for(auto channel : sampleToUse.sample) {
				ss << ofToString(channel) << ", ";
			}
			cout << ofToString(sampleToUse.timestamp,15) << '\t' << ss.str() << std::endl;
		}
//	}

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

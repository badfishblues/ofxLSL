#include "ofMain.h"
#include "ofApp.h"

const char * ofxLSL::resInletInfo = "name = 'CFL'";
//========================================================================
int main() {
	ofSetupOpenGL(1024, 768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
	ofApp app;

	ofSetLogLevel(OF_LOG_VERBOSE);

	app.setup();
	app.update();
}
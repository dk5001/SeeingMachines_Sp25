#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	grabber.listDevices();
	grabber.setDeviceID(0);
	grabber.setup(1280, 720);

	resultImg.allocate(grabber.getWidth(), grabber.getHeight(), OF_IMAGE_COLOR);

	ofSetWindowShape(grabber.getWidth(), grabber.getHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
	grabber.update();
	ofPixels& grabberPixels = grabber.getPixels();		// just reading the color, so beneficial to use reference
	ofPixels& resultPixels = resultImg.getPixels();		// calling the actual pixels inside the image
	for (int y = 0; y < grabber.getHeight(); y++) {
		for (int x = 0; x < grabber.getWidth(); x++) {
			ofColor pixColor = grabberPixels.getColor(x, y);
			resultPixels.setColor(x, y, pixColor);
		}
	}
	resultImg.update();	// not calling a copy. calling the actual pixels inside the image
	//resultImg.setFromPixels(resultPixels);	// this is a copy

	//if (ofGetKeyPressed(' '))
	//{
	//	resultImg.update();		// only update when space is pressed
	//}
}

//--------------------------------------------------------------
void ofApp::draw(){
	resultImg.draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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

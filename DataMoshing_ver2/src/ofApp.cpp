#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	grabber.listDevices();
	grabber.setDeviceID(0);
	grabber.setup(1920, 1080);

	resultImg.allocate(grabber.getWidth(), grabber.getHeight(), OF_IMAGE_COLOR);

	ofSetWindowShape(grabber.getWidth(), grabber.getHeight());

	currentFrame.allocate(grabber.getWidth(), grabber.getHeight(), OF_PIXELS_RGB);
	prevFrame.allocate(grabber.getWidth(), grabber.getHeight(), OF_PIXELS_RGB);
	processedFrame.allocate(grabber.getWidth(), grabber.getHeight(), OF_PIXELS_RGB);

	texture.allocate(grabber.getWidth(), grabber.getHeight(), GL_RGB);

	threshold = 200.0f;

	// Setup GUI
	gui.setup();
	gui.add(thresholdSlider.setup("Threshold", threshold, 0.0f, 255.0f));
	gui.add(pixelSizeSlider.setup("Pixel Size", 10, 1, 50));
}

//--------------------------------------------------------------
void ofApp::update() {
	grabber.update();
	//ofPixels& grabberPixels = grabber.getPixels();		// just reading the color, so beneficial to use reference
	//ofPixels& resultPixels = resultImg.getPixels();		// calling the actual pixels inside the image
	//for (int y = 0; y < grabber.getHeight(); y++) {
	//	for
	//		(int x = 0; x < grabber.getWidth(); x++) {
	//		ofColor pixColor = grabberPixels.getColor(x, y);
	//		resultPixels.setColor(x, y, pixColor);
	//	}
	//}
	//resultImg.update();	// not calling a copy. calling the actual pixels inside the image

	if (grabber.isFrameNew()) {
		currentFrame = grabber.getPixels();

		for (int y = 0; y < currentFrame.getHeight(); y++) {
			for (int x = 0; x < currentFrame.getWidth(); x++) {
				ofColor currentColor = currentFrame.getColor(x, y);
				ofColor prevColor = prevFrame.getColor(x, y);

				// Calculate color difference between frames
				float diff = abs(currentColor.r - prevColor.r) +
					abs(currentColor.g - prevColor.g) +
					abs(currentColor.b - prevColor.b);

				if (diff > thresholdSlider) {
					// Moving pixel: use current color
					processedFrame.setColor(x, y, currentColor);
				}
				else {
					// Static pixel: retain previous color
					processedFrame.setColor(x, y, prevColor);
				}
			}
		}
		// Update texture with processed frame data
		texture.loadData(processedFrame);

		// Store current frame as the previous frame for the next iteration
		prevFrame = currentFrame;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	/*resultImg.draw(0, 0, ofGetWidth(), ofGetHeight());*/
	texture.draw(0, 0);
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

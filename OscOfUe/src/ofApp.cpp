#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    // Initialize webcam
    videoGrabber.setup(640, 480);

    // OSC settings
    oscHost = "192.168.1.164";
    oscPort = 3030;
    oscAddress = "/brightness";
    oscSender.setup(oscHost, oscPort);

    // Brightness calculation settings
    downsampleSize = 32;      // Downsample to 32x32 pixels
    sendInterval = 2;         // Send every 2 frames
    frameCounter = 0;
    currentBrightness = 0.0f;
    lastOscValue = -1;        // Initialize to a value that won't match any valid oscValue

    // Debug visualization
    downsampledImg.allocate(downsampleSize, downsampleSize, OF_IMAGE_COLOR);
    showDebug = true;

    // GUI setup
    gui.setup();
    gui.add(brightnessSlider.setup("Brightness Threshold", 0.1f, 0.0f, 1.0f));

    ofSetFrameRate(30);
    ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update() {
    videoGrabber.update();

    if (videoGrabber.isFrameNew()) {
        frameCounter++;

        // Calculate brightness every frame but only send OSC at intervals
        currentBrightness = calculateBrightness();

        // Determine the value to send based on brightness
        //int oscValue = (currentBrightness < 0.1f) ? 1 : 0;
        int oscValue = 0;
        if (currentBrightness < 0.1f) {
            oscValue = 1;
        }
        // Send OSC message only if the value has changed
        if (oscValue != lastOscValue) {
            ofxOscMessage message;
            message.setAddress(oscAddress);
            message.addIntArg(oscValue);
            oscSender.sendMessage(message);

            lastOscValue = oscValue;

            // Debug output
            ofLogNotice() << "Brightness: " << currentBrightness << " sent value: " << oscValue
                << " to " << oscHost << ":" << oscPort << oscAddress;
        }
    }
}

//--------------------------------------------------------------
float ofApp::calculateBrightness() {
    // Create a downsampled copy of the video frame
    ofPixels originalPixels = videoGrabber.getPixels();
    ofPixels smallPixels;
    smallPixels.allocate(downsampleSize, downsampleSize, OF_IMAGE_COLOR);

    // Resize the image (this is the optimization - we process far fewer pixels)
    ofPixels resizedPixels;
    resizedPixels.allocate(downsampleSize, downsampleSize, OF_IMAGE_COLOR);
    originalPixels.resizeTo(resizedPixels);

    // Store downsampled image for visualization
    if (showDebug) {
        downsampledImg.setFromPixels(resizedPixels);
    }

    // Calculate average brightness
    float totalBrightness = 0.0f;
    int totalPixels = downsampleSize * downsampleSize;

    for (int y = 0; y < downsampleSize; y++) {
        for (int x = 0; x < downsampleSize; x++) {
            ofColor color = resizedPixels.getColor(x, y);

            // Use proper luminance formula (perceived brightness)
            // Y = 0.2126*R + 0.7152*G + 0.0722*B
            float brightness = 0.2126f * color.r + 0.7152f * color.g + 0.0722f * color.b;
            totalBrightness += brightness;
        }
    }

    // Return normalized brightness (0.0 - 1.0)
    return totalBrightness / (totalPixels * 255.0f);
}

//--------------------------------------------------------------
void ofApp::draw() {
    // Draw the webcam feed
    ofSetColor(255);
    videoGrabber.draw(0, 0);

    // Draw debug information if enabled
    if (showDebug) {
        // Draw downsampled image
        downsampledImg.draw(10, 10, 100, 100);

        // Draw brightness value
        ofDrawBitmapStringHighlight("Brightness: " + ofToString(currentBrightness, 3), 10, 130);
        ofDrawBitmapStringHighlight("OSC: " + oscHost + ":" + ofToString(oscPort) + oscAddress, 10, 150);

        // Draw brightness meter
        ofSetColor(50);
        ofDrawRectangle(10, 170, 200, 20);
        ofSetColor(255, 255, 0);
        ofDrawRectangle(10, 170, currentBrightness * 200, 20);
    }

    // Draw GUI
    gui.draw();

    // Instructions
    ofDrawBitmapStringHighlight("Press space to toggle debug view", 10, ofGetHeight() - 20);

    // Toggle debug visualization with 'd' key
    if (ofGetKeyPressed(' ')) {
        showDebug = !showDebug;
    }
}

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    // Calculate average brightness using downsampling
    float calculateBrightness();

    ofVideoGrabber videoGrabber;
    ofxOscSender oscSender;

    // Parameters
    int downsampleSize;       // Size to downsample to (e.g., 32x32)
    int sendInterval;         // Send OSC every N frames
    int frameCounter;         // To track frames for interval sending
    float currentBrightness;  // Store current brightness value
    string oscHost;           // OSC destination host
    int oscPort;              // OSC destination port
    string oscAddress;        // OSC message address
    int lastOscValue;         // Last sent OSC value to avoid sending duplicates

    // Debug visualization
    ofImage downsampledImg;   // For visualization
    bool showDebug;           // Toggle debug visualization

    // GUI
    ofxPanel gui;
    ofxFloatSlider brightnessSlider;
};

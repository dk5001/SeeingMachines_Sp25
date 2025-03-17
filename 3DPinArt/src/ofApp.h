#pragma once

#include "ofMain.h"
#include "ofxRealSense2.h"
#include "ofxAssimpModelLoader.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);

private:
    ofxRealSense2::Context rsContext;
    float elapsedTime;
    float interval;

    float averageDistance; // Store the average distance
    float initialZPosition; // Initial Z position of the model
    float minAverageDistance; // Minimum average distance encountered

    ofVideoGrabber webcam; // Add webcam object

    ofxAssimpModelLoader model; // 3D model loader
    ofEasyCam cam; // Easy camera for 3D navigation
    ofLight light; // Light source
};
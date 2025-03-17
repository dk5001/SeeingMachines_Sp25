#pragma once

#include "ofMain.h"
#include "ofxRealSense2.h"

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
};
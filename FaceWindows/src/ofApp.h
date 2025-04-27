#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofAppFace.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void exit();

    void detectFaces();
    void manageWindows();

    ofVideoGrabber cam;
    ofxCv::ObjectFinder faceFinder;

    int nextFaceID = 0;
    std::map<int, shared_ptr<ofAppFace>> faceWindows; // Face ID -> App instance
    std::map<int, ofRectangle> previousFaces;         // Face ID -> Face bounds
};

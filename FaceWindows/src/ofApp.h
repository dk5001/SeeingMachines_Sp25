#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofAppFace.h"
#include "FeatureType.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void exit();

    void detectFaces();
    void manageWindows();
    void updateWindowPosition();

    ofVideoGrabber cam;
    ofxCv::ObjectFinder faceFinder;

    int nextFaceID = 0;

    struct FeatureWindow {
        shared_ptr<ofAppFace> app;
        GLFWwindow* glfw;
    };

    std::map<int, std::map<FeatureType, FeatureWindow>> faceWindows;
    // Face ID -> App instance
    std::map<int, ofRectangle> previousFaces;         // Face ID -> Face bounds
    glm::vec2 windowPosition;
};

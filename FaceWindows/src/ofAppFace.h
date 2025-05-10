#pragma once

#include "ofMain.h"
#include "FeatureType.h"

class ofAppFace : public ofBaseApp {
public:
    void setupFace(ofVideoGrabber& sourceCam, ofRectangle faceRect, FeatureType type, int width, int height);
    void updateFace(ofVideoGrabber& sourceCam, ofRectangle faceRect);
    void setUpdating(bool updating);
    void update();
    void draw();
    void windowResized(int w, int h);

private:
    ofImage faceCrop;
    bool isUpdating = true;
    FeatureType featureType;

    ofRectangle getFeatureROI(ofRectangle faceRect);

    glm::vec2 windowSize;
    int faceWidth;
    int faceHeight;
};

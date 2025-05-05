#pragma once

#include "ofMain.h"
#include "FeatureType.h"

class ofAppFace : public ofBaseApp {
public:
    void setupFace(ofVideoGrabber& sourceCam, ofRectangle faceRect, FeatureType type);
    void updateFace(ofVideoGrabber& sourceCam, ofRectangle faceRect);
    void setUpdating(bool updating);
    void update();
    void draw();

private:
    ofImage faceCrop;
    bool isUpdating = true;
    FeatureType featureType;

    ofRectangle getFeatureROI(ofRectangle faceRect);
};

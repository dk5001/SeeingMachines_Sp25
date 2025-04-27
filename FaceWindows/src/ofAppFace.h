#pragma once

#include "ofMain.h"

class ofAppFace : public ofBaseApp {
public:
    void setupFace(ofVideoGrabber& sourceCam, ofRectangle faceRect);
    void updateFace(ofVideoGrabber& sourceCam, ofRectangle faceRect);
    void setUpdating(bool updating);

    void update();
    void draw();

private:
    ofImage faceCrop;
    bool isUpdating = true; // Whether this face window should update
};

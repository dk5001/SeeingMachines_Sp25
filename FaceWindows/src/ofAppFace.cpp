#include "ofAppFace.h"

void ofAppFace::setupFace(ofVideoGrabber& sourceCam, ofRectangle faceRect) {
    updateFace(sourceCam, faceRect);
}

void ofAppFace::updateFace(ofVideoGrabber& sourceCam, ofRectangle faceRect) {
    if (!isUpdating) return; // Freeze frame if not updating

    faceCrop.setFromPixels(sourceCam.getPixels());
    faceCrop.crop(faceRect.x, faceRect.y, faceRect.width, faceRect.height);
    faceCrop.resize(200, 200);
}

void ofAppFace::update() {
    // Nothing special in update right now
}

void ofAppFace::draw() {
    faceCrop.draw(0, 0);
}

void ofAppFace::setUpdating(bool updating) {
    isUpdating = updating;
}

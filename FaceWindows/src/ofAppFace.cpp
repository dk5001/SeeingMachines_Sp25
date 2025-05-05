#include "ofAppFace.h"

void ofAppFace::setupFace(ofVideoGrabber& sourceCam, ofRectangle faceRect, FeatureType type) {
    featureType = type;
    updateFace(sourceCam, faceRect);
}

void ofAppFace::updateFace(ofVideoGrabber& sourceCam, ofRectangle faceRect) {
    if (!isUpdating) return;

    auto roi = getFeatureROI(faceRect);
    faceCrop.setFromPixels(sourceCam.getPixels());
    faceCrop.crop(roi.x, roi.y, roi.width, roi.height);
    faceCrop.resize(200, 200);
}

void ofAppFace::update() {}

void ofAppFace::draw() {
    faceCrop.draw(0, 0);
}

void ofAppFace::setUpdating(bool updating) {
    isUpdating = updating;
}

ofRectangle ofAppFace::getFeatureROI(ofRectangle faceRect) {
    float x = faceRect.x;
    float y = faceRect.y;
    float w = faceRect.width;
    float h = faceRect.height;

    switch (featureType) {
    case FeatureType::EYE:
        return ofRectangle(x + w * 0.2, y + h * 0.2, w * 0.6, h * 0.3);
    case FeatureType::NOSE:
        return ofRectangle(x + w * 0.35, y + h * 0.4, w * 0.3, h * 0.3);
    case FeatureType::MOUTH:
        return ofRectangle(x + w * 0.3, y + h * 0.7, w * 0.4, h * 0.2);
    default:
        return faceRect;
    }
}

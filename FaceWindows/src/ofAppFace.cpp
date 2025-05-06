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
    case FeatureType::EYE_LEFT:
        return ofRectangle(x + w * 0.2, y + h * 0.25, w * 0.25, h * 0.2);
    case FeatureType::EYE_RIGHT:
        return ofRectangle(x + w * 0.55, y + h * 0.25, w * 0.25, h * 0.2);
    case FeatureType::NOSE:
        return ofRectangle(x + w * 0.35, y + h * 0.45, w * 0.3, h * 0.25);
    case FeatureType::MOUTH:
        return ofRectangle(x + w * 0.3, y + h * 0.75, w * 0.4, h * 0.2);
    default:
        return faceRect;
    }
}

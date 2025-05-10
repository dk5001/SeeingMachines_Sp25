#include "ofAppFace.h"

void ofAppFace::setupFace(ofVideoGrabber& sourceCam, ofRectangle faceRect, FeatureType type, int width, int height) {
    featureType = type;
    faceWidth = width;
    faceHeight = height;
    updateFace(sourceCam, faceRect);
    windowSize = glm::vec2(width, height);
}

void ofAppFace::updateFace(ofVideoGrabber& sourceCam, ofRectangle faceRect) {
    if (!isUpdating) return;

    const ofPixels& camPixels = sourceCam.getPixels();
    if (!camPixels.isAllocated() || camPixels.getWidth() == 0 || camPixels.getHeight() == 0) return;

    ofRectangle roi = getFeatureROI(faceRect);

    // Clamp ROI to image bounds
    roi.x = std::max(0.f, roi.x);
    roi.y = std::max(0.f, roi.y);
    roi.width = std::min(roi.width, camPixels.getWidth() - roi.x);
    roi.height = std::min(roi.height, camPixels.getHeight() - roi.y);

    if (roi.width <= 0 || roi.height <= 0) return;

    faceCrop.cropFrom(sourceCam.getPixels(), roi.x, roi.y, roi.width, roi.height);
    faceCrop.update();

    ofLogNotice() << "ROI: " << roi;
}

void ofAppFace::update() {}

void ofAppFace::draw() {
    //faceCrop.draw(0, 0);
    faceCrop.draw(0, 0, faceWidth, faceHeight);
}

void ofAppFace::windowResized(int w, int h)
{
    windowSize = glm::vec2(w, h);
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

#include "ofApp.h"
#include <GLFW/glfw3.h>

void ofApp::setup() {
    cam.setup(640, 480);

    faceFinder.setup("haarcascade_frontalface_default.xml");
    faceFinder.setPreset(ofxCv::ObjectFinder::Fast);
}

void ofApp::update() {
    cam.update();
    if (cam.isFrameNew()) {
        detectFaces();
        manageWindows();
    }
}

void ofApp::draw() {
    //cam.draw(0, 0);

    // Draw rectangles around detected faces
    for (auto& face : previousFaces) {
        //ofNoFill();
        //ofSetColor(255, 0, 0);
        ofDrawRectangle(face.second);
    }
}

void ofApp::detectFaces() {
    faceFinder.update(cam);

    // Temporary map for this frame
    std::map<int, ofRectangle> detectedFaces;

    for (size_t i = 0; i < faceFinder.size(); i++) {
        ofRectangle faceRect = faceFinder.getObjectSmoothed(static_cast<unsigned int>(i));

        // Try to match to existing face
        bool matched = false;
        for (auto& prev : previousFaces) {
            if (glm::distance(faceRect.getCenter(), prev.second.getCenter()) < 50) {
                detectedFaces[prev.first] = faceRect;
                matched = true;
                break;
            }
        }

        if (!matched) {
            detectedFaces[nextFaceID++] = faceRect;
        }
    }

    previousFaces = detectedFaces;
}

void ofApp::manageWindows() {
    for (auto& face : previousFaces) {
        int id = face.first;
        auto rect = face.second;

        for (auto feature : { FeatureType::EYE, FeatureType::NOSE, FeatureType::MOUTH }) {
            auto key = id * 10 + static_cast<int>(feature);
            if (faceWindows.find(key) == faceWindows.end()) {
                ofGLFWWindowSettings settings;
                settings.width = 200;
                settings.height = 200;
                settings.resizable = false;
                settings.setPosition({ rect.x + 250 * static_cast<int>(feature), rect.y });

                auto win = ofCreateWindow(settings);
                auto app = make_shared<ofAppFace>();
                app->setupFace(cam, rect, feature);
                ofRunApp(win, app);
                faceWindows[key] = app;
            }
            else {
                faceWindows[key]->setUpdating(true);
                faceWindows[key]->updateFace(cam, rect);
            }
        }
    }

    // Freeze unmatched
    std::set<int> activeKeys;
    for (auto& face : previousFaces)
        for (int i = 0; i < 3; ++i)
            activeKeys.insert(face.first * 10 + i);

    for (auto& win : faceWindows)
        if (activeKeys.find(win.first) == activeKeys.end())
            win.second->setUpdating(false);
}

void ofApp::exit() {
    // Cleanup all face windows
    //for (auto& face : faceWindows) {
    //    face.second->requestClose();
    //}
}

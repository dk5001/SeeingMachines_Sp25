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
    const int winWidth = 200;
    const int winHeight = 200;
    const int verticalSpacing = 5;

    for (auto& [id, faceRect] : previousFaces) {
        int baseX = static_cast<int>(faceRect.x + faceRect.width + 20);
        int baseY = static_cast<int>(faceRect.y);

        int featureIndex = 0;

        for (auto feature : { FeatureType::EYE, FeatureType::NOSE, FeatureType::MOUTH }) {
            if (faceWindows[id].find(feature) == faceWindows[id].end()) {
                // Create new window
                ofGLFWWindowSettings settings;
                settings.width = winWidth;
                settings.height = winHeight;
                settings.resizable = false;
                settings.setPosition({ baseX, baseY + featureIndex * (winHeight + verticalSpacing) });

                auto win = ofCreateWindow(settings);
                auto app = make_shared<ofAppFace>();
                app->setupFace(cam, faceRect, feature);
                ofRunApp(win, app);

                auto glfwWin = dynamic_cast<ofAppGLFWWindow*>(win.get())->getGLFWWindow();
                faceWindows[id][feature] = { app, glfwWin };
            }
            else {
                auto& fw = faceWindows[id][feature];
                fw.app->setUpdating(true);
                fw.app->updateFace(cam, faceRect);

                // Move the window to track vertically relative to face
                int winX = baseX;
                int winY = baseY + featureIndex * (winHeight + verticalSpacing);
                glfwSetWindowPos(fw.glfw, winX, winY);
            }

            ++featureIndex;
        }
    }

    // Freeze unmatched
    for (auto& [id, features] : faceWindows) {
        if (previousFaces.find(id) == previousFaces.end()) {
            for (auto& [feature, fw] : features) {
                fw.app->setUpdating(false);
            }
        }
    }
}


void ofApp::exit() {
    // Cleanup all face windows
    //for (auto& face : faceWindows) {
    //    face.second->requestClose();
    //}
}

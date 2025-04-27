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
    // Open new windows for newly detected faces
    for (auto& face : previousFaces) {
        if (faceWindows.find(face.first) == faceWindows.end()) {
            // Create new window
            ofGLFWWindowSettings settings;
            settings.width = 200;
            settings.height = 200;
            settings.resizable = false;

            // Set initial position of the window based on the face's position
            settings.setPosition(ofVec2f(face.second.getX(), face.second.getY()));

            auto win = ofCreateWindow(settings);

            auto faceApp = make_shared<ofAppFace>();
            faceApp->setupFace(cam, face.second);

            ofRunApp(win, faceApp);
            faceWindows[face.first] = faceApp;
        } else {
            // Update the face normally
            faceWindows[face.first]->setUpdating(true);
            faceWindows[face.first]->updateFace(cam, face.second);

            // Move the window to follow the face's position
            auto baseWindow = ofGetWindowPtr();
            auto glfwWindow = dynamic_cast<ofAppGLFWWindow*>(baseWindow);
            if (glfwWindow) {
                auto newPosition = ofVec2f(face.second.getX(), face.second.getY());
                glfwSetWindowPos(glfwWindow->getGLFWWindow(), static_cast<int>(newPosition.x), static_cast<int>(newPosition.y));
            }
        }
    }

    // For any window not matched, freeze (but don't close)
    for (auto& face : faceWindows) {
        if (previousFaces.find(face.first) == previousFaces.end()) {
            face.second->setUpdating(false); // Stop updating, freeze frame
        }
    }
}

void ofApp::exit() {
    // Cleanup all face windows
    //for (auto& face : faceWindows) {
    //    face.second->requestClose();
    //}
}

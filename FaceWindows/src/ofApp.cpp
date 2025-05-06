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

    std::vector<FeatureType> features = {
    FeatureType::EYE_LEFT,
    FeatureType::EYE_RIGHT,
    FeatureType::NOSE,
    FeatureType::MOUTH
    };

    for (auto& [id, faceRect] : previousFaces) {
        int baseX = static_cast<int>(faceRect.x + faceRect.width + 20);
        int baseY = static_cast<int>(faceRect.y);

        for (int i = 0; i < features.size(); ++i) {
            auto feature = features[i];

            // Horizontal layout for eyes
            int offsetX = baseX;
            int offsetY = baseY;

            if (feature == FeatureType::EYE_LEFT) {
                offsetX = baseX - 110;  // Shift left eye slightly to the left
                offsetY = baseY;
            }
            else if (feature == FeatureType::EYE_RIGHT) {
                offsetX = baseX + 110;  // Shift right eye to the right
                offsetY = baseY;
            }
            else if (feature == FeatureType::NOSE) {
                offsetX = baseX;
                offsetY = baseY + 210;
            }
            else if (feature == FeatureType::MOUTH) {
                offsetX = baseX;
                offsetY = baseY + 420;
            }


            if (faceWindows[id].find(feature) == faceWindows[id].end()) {
                ofGLFWWindowSettings settings;
                settings.width = 200;
                settings.height = 200;
                settings.resizable = false;
                settings.setPosition({ offsetX, offsetY });

                auto win = ofCreateWindow(settings);
                auto app = std::make_shared<ofAppFace>();
                app->setupFace(cam, faceRect, feature);
                ofRunApp(win, app);

                auto glfw = dynamic_cast<ofAppGLFWWindow*>(win.get())->getGLFWWindow();

                // Set the window title based on the feature type -> not working 
                std::string title;
                switch (feature) {
                case FeatureType::EYE_LEFT:  title = "Left Eye"; break;
                case FeatureType::EYE_RIGHT: title = "Right Eye"; break;
                case FeatureType::NOSE:      title = "Nose"; break;
                case FeatureType::MOUTH:     title = "Mouth"; break;
                }
                glfwSetWindowTitle(glfw, title.c_str());

                faceWindows[id][feature] = { app, glfw };
            }
            else {
                auto& fw = faceWindows[id][feature];
                fw.app->setUpdating(true);
                fw.app->updateFace(cam, faceRect);
                glfwSetWindowPos(fw.glfw, offsetX, offsetY);
            }
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

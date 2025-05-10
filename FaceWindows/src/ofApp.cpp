#include "ofApp.h"
#include <GLFW/glfw3.h>

void ofApp::setup() {
    cam.setup(640, 480);

    faceFinder.setup("haarcascade_frontalface_default.xml");
    faceFinder.setPreset(ofxCv::ObjectFinder::Fast);
    faceFinder.setFindBiggestObject(false);
}

void ofApp::update() {
    cam.update();
    if (cam.isFrameNew()) {
        detectFaces();
        manageWindows();
    }
}

void ofApp::draw() {
    cam.draw(0, 0);
    
    faceFinder.draw(); // Draw rectangles around detected faces

    // ----------------------------------------------
    // Draw rectangles around detected faces
    //for (auto& face : previousFaces) {
    //    //ofNoFill();
    //    //ofSetColor(255, 0, 0);
    //    ofDrawRectangle(face.second);
    //}
}

void ofApp::detectFaces() {
    faceFinder.update(cam);

    // ----------------------------------------------
    //// Temporary map for this frame
    //std::map<int, ofRectangle> detectedFaces;

    //for (size_t i = 0; i < faceFinder.size(); i++) {
    //    ofRectangle faceRect = faceFinder.getObjectSmoothed(static_cast<unsigned int>(i));

    //    // Try to match to existing face
    //    bool matched = false;
    //    for (auto& prev : previousFaces) {
    //        if (glm::distance(faceRect.getCenter(), prev.second.getCenter()) < 50) {
    //            detectedFaces[prev.first] = faceRect;
    //            matched = true;
    //            break;
    //        }
    //    }

    //    if (!matched) {
    //        detectedFaces[nextFaceID++] = faceRect;
    //    }
    //}

    //previousFaces = detectedFaces;
    // ----------------------------------------------

    // Update the map of detected faces
    previousFaces.clear();
    for (int i = 0; i < faceFinder.size(); i++) {
        ofRectangle faceRect = faceFinder.getObjectSmoothed(i);
        previousFaces[i] = faceRect; // Store the face rectangle with an ID
    }
}

void ofApp::manageWindows() {
    const int eyeWidth = 150;
    const int eyeHeight = 150;
    const int noseWidth = 100;
    const int noseHeight = 200;
    const int mouthWidth = 300;
    const int mouthHeight = 100;
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
            int winWidth = 200;  // Default width
            int winHeight = 200; // Default height

            if (feature == FeatureType::EYE_LEFT) {
                offsetX = baseX - 110;  // Shift left eye slightly to the left
                offsetY = baseY;
                winWidth = eyeWidth;
                winHeight = eyeHeight;
            }
            else if (feature == FeatureType::EYE_RIGHT) {
                offsetX = baseX + 110;  // Shift right eye to the right
                offsetY = baseY;
                winWidth = eyeWidth;
                winHeight = eyeHeight;
            }
            else if (feature == FeatureType::NOSE) {
                offsetX = baseX;
                offsetY = baseY + 210;
                winWidth = noseWidth;
                winHeight = noseHeight;
            }
            else if (feature == FeatureType::MOUTH) {
                offsetX = baseX;
                offsetY = baseY + 420;
                winWidth = mouthWidth;
                winHeight = mouthHeight;
            }

            if (faceWindows[id].find(feature) == faceWindows[id].end()) {
                ofGLFWWindowSettings settings;
                settings.width = winWidth;  // Use custom width
                settings.height = winHeight; // Use custom height
                settings.resizable = false;
                settings.setPosition({ offsetX, offsetY });

                auto win = ofCreateWindow(settings);
                auto app = std::make_shared<ofAppFace>();
                app->setupFace(cam, faceRect, feature, winWidth, winHeight); // Pass custom size
                ofRunApp(win, app);

                auto glfw = dynamic_cast<ofAppGLFWWindow*>(win.get())->getGLFWWindow();

                // Set the window title based on the feature type
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

void ofApp::windowResized(int w, int h)
{
// Handle window resizing for each face window
    for (auto& [id, features] : faceWindows) {
        for (auto& [feature, fw] : features) {
            fw.app->windowResized(w, h);
        }
    }
}

void ofApp::exit() {
    // Cleanup all face windows
    //for (auto& face : faceWindows) {
    //    face.second->requestClose();
    //}
}

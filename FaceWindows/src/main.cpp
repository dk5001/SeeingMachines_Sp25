#include "ofMain.h"
#include "ofApp.h"

int main() {
    ofSetupOpenGL(1280, 720, OF_WINDOW); // Main webcam window
    ofRunApp(new ofApp());
}

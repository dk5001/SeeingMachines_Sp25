#include "ofApp.h"

void ofApp::setup()
{
    // Default RS resolution.
    ofSetWindowShape(640, 360);

    rsContext.setup(true);

    // Initialize timer
    elapsedTime = 0.0f;

    // Set up the webcam
    webcam.setup(640, 360); // Set the resolution for the webcam

    // Load the 3D model
    model.loadModel("AILogo.obj"); // Replace with your model file path
    model.setPosition(0, 0, 0); // Set initial position to the origin
    model.setScale(0.5, 0.5, 0.5); // Adjust the scale as needed

    // Enable smooth lighting
    ofEnableSmoothing();
    ofEnableDepthTest();

    // Set up the light
    light.setup();
    light.setPosition(0, 0, 600); // Position the light on the Z-axis
    light.lookAt(ofVec3f(0, 0, 0)); // Point the light towards the origin (front of the model)
    light.enable();
}

void ofApp::update()
{
    rsContext.update();

    // Update elapsed time
    //elapsedTime += ofGetLastFrameTime();
    webcam.update(); // Update the webcam frame

    // Update the model (if it has animations)
    model.update();

    std::shared_ptr<ofxRealSense2::Device> rsDevice = rsContext.getDevice(0);
    if (rsDevice)
    {
        // Get the depth texture
        ofTexture depthTex = rsDevice->getDepthTex();

        // Calculate the center of the window
        int centerX = ofGetWidth() / 2;
        int centerY = ofGetHeight() / 2;

        // Define the square area
        int squareWidth = 64;
        int squareHeight = 36;
        int startX = centerX - squareWidth / 2;
        int startY = centerY - squareHeight / 2;

        // Calculate the average distance
        float totalDistance = 0.0f;
        int pixelCount = 0;

        for (int y = startY; y < startY + squareHeight; ++y)
        {
            for (int x = startX; x < startX + squareWidth; ++x)
            {
                float distance = rsDevice->getDistance(x, y);
                if (distance > 0) // Ignore invalid distances
                {
                    totalDistance += distance;
                    ++pixelCount;
                }
            }
        }

        if (pixelCount > 0)
        {
            averageDistance = totalDistance / pixelCount;
            ofLogNotice() << "Average Distance: " << averageDistance;
        }
    }
}

void ofApp::draw()
{
    //// Clear the background
    //ofBackground(0);
    //webcam.draw(0, 0, ofGetWidth(), ofGetHeight());

    // Map the average distance to an opacity range
    int opacity = ofMap(averageDistance, 0, 1, 255, 0, true); // Adjust the range as needed
    ofLogNotice() << "opacity: " << opacity;

    //// Set the color with the calculated opacity
    //ofSetColor(128, 128, 128, opacity); // Greyscale color with variable opacity

    // Set the background color
    ofBackground(0, 0, 0);

    // Begin the camera
    cam.begin();

    // Enable lighting
    ofEnableLighting();
    light.enable();

    // Apply transformations to the model
    ofPushMatrix();
    ofRotateXDeg(180); 
    ofRotateYDeg(180);

    // Draw the model
    model.drawFaces();

    // Restore the previous transformation state
    ofPopMatrix();

    // Disable lighting
    light.disable();
    ofDisableLighting();

    // End the camera
    cam.end();
}

void ofApp::keyPressed(int key)
{
    // No key press handling needed
}

void ofApp::keyReleased(int key)
{
    // No key release handling needed
}
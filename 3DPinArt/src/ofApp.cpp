#include "ofApp.h"

void ofApp::setup()
{
    // Default RS resolution.
    ofSetWindowShape(640, 360);

    rsContext.setup(true);

    // Initialize timer
    elapsedTime = 0.0f;
    interval = .3f; // 1 second
}

void ofApp::update()
{
    rsContext.update();

    // Update elapsed time
    elapsedTime += ofGetLastFrameTime();

    if (elapsedTime >= interval)
    {
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

        // Reset elapsed time
        elapsedTime = 0.0f;
    }
}

void ofApp::draw()
{
    std::shared_ptr<ofxRealSense2::Device> rsDevice = rsContext.getDevice(0);
    if (rsDevice)
    {
        // Get the depth texture
        ofTexture depthTex = rsDevice->getDepthTex();

        // Calculate the new width and height for 1% resolution
        int newWidth = depthTex.getWidth() * 0.01;
        int newHeight = depthTex.getHeight() * 0.01;

        // Create a low-resolution image
        ofImage lowResImage;
        lowResImage.allocate(newWidth, newHeight, OF_IMAGE_GRAYSCALE);
        depthTex.readToPixels(lowResImage.getPixels());
        lowResImage.update();

        // Draw the low-resolution image scaled up to the window size
        //lowResImage.draw(0, 0, ofGetWidth(), ofGetHeight());

        float distAtMouse = rsDevice->getDistance(ofGetMouseX(), ofGetMouseY());
        ofDrawBitmapStringHighlight(ofToString(distAtMouse, 3), ofGetMouseX(), ofGetMouseY());
    }

    // Map the average distance to an opacity range
    int opacity = ofMap(averageDistance, 0, 1, 255, 0, true); // Adjust the range as needed
    ofLogNotice() << "opacity: " << opacity;

    // Set the color with the calculated opacity
    ofSetColor(128, 128, 128, opacity); // Greyscale color with variable opacity

    //// Draw the circle in the middle of the window
    //ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, 50);
}

void ofApp::keyPressed(int key)
{
    // No key press handling needed
}

void ofApp::keyReleased(int key)
{
    // No key release handling needed
}
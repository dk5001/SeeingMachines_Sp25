// ofApp.cpp
#include "ofApp.h"

void ofApp::setup()
{
    ofSetWindowShape(640, 480);

    // Make the circle smoother by increasing the resolution.
    ofSetCircleResolution(128);

    // Set up the OSC sender.
    sendAddr = "192.168.1.164";
    sendPort = 3030;
    sender.setup(sendAddr, sendPort);
}

void ofApp::draw()
{
    ofBackground(0);

    // Draw a circle at the mouse position.
    if (ofGetMousePressed())
    {
        ofFill();
    }
    else
    {
        ofNoFill();
    }
    ofSetColor(255);
    ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 50);
    ofFill();
}

void ofApp::keyPressed(int key)
{
    //if (key == ' ')
    //{
    //    ofxOscMessage msg;
    //    msg.setAddress("/cursor/color");

    //    sender.sendMessage(msg);

    //    //// Print the OSC message details in the console.
    //    //cout << "OSC message sent!" << endl;
    //    //cout << "Address: " << msg.getAddress() << endl;
    //    //cout << "Num Arguments: " << msg.getNumArgs() << endl;
    //    //for (int i = 0; i < msg.getNumArgs(); i++)
    //    //{
    //    //    cout << "Argument " << i << ": " << msg.getArgAsString(i) << endl;
    //    //}

    //    //// Print the sendAddr in the console.
    //    //cout << "Message sent to: " << sendAddr << endl;cout << "------------------------" << endl;
    //}
}

void ofApp::mouseDragged(int x, int y, int button)
{
    //ofxOscMessage msg;
    //msg.setAddress("/cursor/move");
    //msg.addIntArg(ofGetMouseX());
    ////msg.addIntArg(ofGetMouseY());

    //sender.sendMessage(msg);

    //// Print the OSC message details in the console.
    //cout << "Address: " << msg.getAddress() << endl;
    //cout << "X Value: " << ofGetMouseX() << endl;
    ////cout << "Y Value: " << ofGetMouseY() << endl;
    //cout << "Message sent to: " << sendAddr << endl;
    //cout << "------------------------" << endl;
}

void ofApp::mousePressed(int x, int y, int button)
{
    ofxOscMessage msg;
    msg.setAddress("/cursor/press");
    msg.addIntArg(1); // Emit 1 when pressed

    sender.sendMessage(msg);

    // Print the OSC message details in the console.
    cout << "OSC message sent!" << endl;
    cout << "Address: " << msg.getAddress() << endl;
    cout << "Value: 1" << endl;
    cout << "Message sent to: " << sendAddr << endl;
    cout << "------------------------" << endl;
}

void ofApp::mouseReleased(int x, int y, int button)
{
    ofxOscMessage msg;
    msg.setAddress("/cursor/press");
    msg.addIntArg(0); // Emit 0 when not pressed

    sender.sendMessage(msg);

    // Print the OSC message details in the console.
    cout << "OSC message sent!" << endl;
    cout << "Address: " << msg.getAddress() << endl;
    cout << "Value: 0" << endl;
    cout << "Message sent to: " << sendAddr << endl;
    cout << "------------------------" << endl;
}

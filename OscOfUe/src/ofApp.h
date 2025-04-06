// ofApp.h
#pragma once

#include "ofMain.h"

#include "ofxOsc.h"

class ofApp : public ofBaseApp
{
public:
	void setup();
	void draw();

	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	std::string sendAddr;
	int sendPort;

	ofxOscSender sender;
};
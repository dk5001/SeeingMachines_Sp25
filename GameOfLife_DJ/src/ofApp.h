#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
    public:
        void setup();
        void update();
        void draw();

        void keyPressed(int key);
        void mousePressed(int x, int y, int button);

        // Grid properties
        vector<vector<int>> grid;
        int cols, rows;
        int cellSize;

        // Image for starting pattern
        ofImage startImage;

        // GUI elements
        ofxPanel gui;
        ofxIntSlider frameRateSlider;
        ofxIntSlider cellSizeSlider;
        ofxIntSlider thresholdSlider;

        // Helper functions
        void initializeGrid();
        void initializeGridFromImage();
        int countNeighbors(int x, int y);
        void updateGrid();
};

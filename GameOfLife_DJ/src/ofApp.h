#pragma once

#include "ofMain.h"

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

        // Helper functions
        void initializeGrid();
        int countNeighbors(int x, int y);
        void updateGrid();
};

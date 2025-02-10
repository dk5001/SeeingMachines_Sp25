#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    // Set the frame rate
    ofSetFrameRate(10);  // Adjust as needed

    // Initialize grid properties
    cellSize = 5;
    cols = ofGetWidth() / cellSize;
    rows = ofGetHeight() / cellSize;

    initializeGrid();
}

//--------------------------------------------------------------
void ofApp::update() {
    updateGrid();
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(255);  // White background

    for (int W = 0; W < cols; W++) {
        for (int H = 0; H < rows; H++) {
            if (grid[W][H] == 1) {
                ofSetColor(255);  // White for live cells
            }
            else {
                ofSetColor(0);  // Black for dead cells
            }
            ofDrawRectangle(W * cellSize, H * cellSize, cellSize, cellSize);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == ' ') {
        // Spacebar to reset the grid
        initializeGrid();
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    // Toggle cell state on mouse click
    int W = x / cellSize;
    int H = y / cellSize;
    grid[W][H] = 1 - grid[W][H];
}

//--------------------------------------------------------------
void ofApp::initializeGrid() {
    grid.resize(cols, vector<int>(rows, 0));
    // Randomly initialize the grid
    for (int W = 0; W < cols; W++) {
        for (int H = 0; H < rows; H++) {
            grid[W][H] = ofRandom(2);  // 0 or 1
        }
    }
}

//--------------------------------------------------------------
int ofApp::countNeighbors(int x, int y) {
    int count = 0;
    for (int W = -1; W <= 1; W++) {
        for (int H = -1; H <= 1; H++) {
            int col = (x + W + cols) % cols;
            int row = (y + H + rows) % rows;
            count += grid[col][row];
        }
    }
    count -= grid[x][y];  // Subtract the cell itself
    return count;
}

//--------------------------------------------------------------
void ofApp::updateGrid() {
    vector<vector<int>> next(cols, vector<int>(rows, 0));

    for (int W = 0; W < cols; W++) {
        for (int H = 0; H < rows; H++) {
            int neighbors = countNeighbors(W, H);
            int state = grid[W][H];

            if (state == 0 && neighbors == 3) {
                next[W][H] = 1;  // Reproduction
            }
            else if (state == 1 && (neighbors < 2 || neighbors > 3)) {
                next[W][H] = 0;  // Death
            }
            else {
                next[W][H] = state;  // Stasis
            }
        }
    }

    grid = next;  // Update the grid
}

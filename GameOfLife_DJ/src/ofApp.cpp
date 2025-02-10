#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    // Set the frame rate
    ofSetFrameRate(10);  // Adjust as needed

    // Initialize grid properties
    cellSize = 10;
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

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            if (grid[i][j] == 1) {
                ofSetColor(255);  // White for live cells
            }
            else {
                ofSetColor(0);  // Black for dead cells
            }
            ofDrawRectangle(i * cellSize, j * cellSize, cellSize, cellSize);
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
    int i = x / cellSize;
    int j = y / cellSize;
    grid[i][j] = 1 - grid[i][j];
}

//--------------------------------------------------------------
void ofApp::initializeGrid() {
    grid.resize(cols, vector<int>(rows, 0));
    // Randomly initialize the grid
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            grid[i][j] = ofRandom(2);  // 0 or 1
        }
    }
}

//--------------------------------------------------------------
int ofApp::countNeighbors(int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int col = (x + i + cols) % cols;
            int row = (y + j + rows) % rows;
            count += grid[col][row];
        }
    }
    count -= grid[x][y];  // Subtract the cell itself
    return count;
}

//--------------------------------------------------------------
void ofApp::updateGrid() {
    vector<vector<int>> next(cols, vector<int>(rows, 0));

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            int neighbors = countNeighbors(i, j);
            int state = grid[i][j];

            if (state == 0 && neighbors == 3) {
                next[i][j] = 1;  // Reproduction
            }
            else if (state == 1 && (neighbors < 2 || neighbors > 3)) {
                next[i][j] = 0;  // Death
            }
            else {
                next[i][j] = state;  // Stasis
            }
        }
    }

    grid = next;  // Update the grid
}

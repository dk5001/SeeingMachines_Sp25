#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    // Print the rules
    cout << "GAME OF LIFE" << endl;
    cout << "THE RULES: " << endl;
    cout << "FOR A SPACE THAT IS 'POPULATED':" << endl;
    cout << "* EACH CELL WITH ONE OR NO NEIGHBORS DIES, AS IF BY SOLITUDE." << endl;
    cout << "* EACH CELL WITH FOUR OR NO NEIGHBORS DIES, AS IF BY OVERPOPULATION." << endl;
    cout << "* EACH CELL WITH TWO OR THREE NEIGHBORS SURVIVES." << endl;
    cout << "FOR A SPACE THAT IS 'EMPTY' OR 'UNPOPULATED':" << endl;
    cout << "* EACH CELL WITH THREE NEIGHBORS BECOMES POPULATED." << endl;
    cout << endl;
    cout << "INSTRUCTIONS: " << endl;
    cout << "PRESS SPACE TO RESET" << endl;
    cout << "CLICK A CELL TO TOGGLE STATE" << endl;

    // Set the frame rate
    ofSetFrameRate(5); 


    // Load the starting image
    startImage.load("startPattern.jpg");
    
    // Resize the image to fit the grid
    int imageWidth = startImage.getWidth();
    int imageHeight = startImage.getHeight();

    // Set the window size based on the image dimensions
    ofSetWindowShape(imageWidth, imageHeight);
    
    // Initialize grid properties
    cellSize = 2;
    cols = ofGetWidth() / cellSize;
    rows = ofGetHeight() / cellSize;

    // Resize the image to fit the grid
    startImage.resize(cols, rows);

    initializeGridFromImage();

    // Setup GUI
    gui.setup();
    gui.add(frameRateSlider.setup("Frame Rate", 10, 1, 60));
    //gui.add(cellSizeSlider.setup("Cell Size", 5, 1, 20));
    gui.add(thresholdSlider.setup("Threshold", 50, 0, 255));
}

//--------------------------------------------------------------
void ofApp::update() {
    ofSetFrameRate(frameRateSlider);

    //// Check if cell size has changed
    //if (cellSize != cellSizeSlider) {
    //    cellSize = cellSizeSlider;
    //    cols = ofGetWidth() / cellSize;
    //    rows = ofGetHeight() / cellSize;
    //    startImage.resize(cols, rows);
    //    initializeGridFromImage();
    //}
    
    updateGrid();
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(255); 

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

    // Draw GUI
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == ' ') {
        // Spacebar to reset the grid
        initializeGridFromImage();
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
void ofApp::initializeGridFromImage() {
    grid.resize(cols, vector<int>(rows, 0));
    ofPixels &pixels = startImage.getPixels();
    for (int W = 0; W < cols; W++) {
        for (int H = 0; H < rows; H++) {
            ofColor color = pixels.getColor(W, H);
            grid[W][H] = (color.getBrightness() > thresholdSlider) ? 0 : 1;  // Threshold to determine live or dead cell
        }
    }
}

// Update the initialization of the grid from the image
//void ofApp::initializeGridFromImage() {
//    grid.resize(cols, vector<int>(rows, 0));
//    ofPixels &pixels = startImage.getPixels();
//    for (int W = 0; W < cols; W++) {
//        for (int H = 0; H < rows; H++) {
//            ofColor color = pixels.getColor(W, H);
//            grid[W][H] = (color.getBrightness() > thresholdSlider) ? 0 : 1;  // Threshold to determine live or dead cell
//        }
//    }
//}

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

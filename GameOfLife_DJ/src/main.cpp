#include "ofMain.h"
#include "ofApp.h"

//The Rules

//For a space that is 'populated':
//Each cell with one or no neighbors dies, as if by solitude.
//Each cell with four or more neighbors dies, as if by overpopulation.
//Each cell with two or three neighbors survives.
//For a space that is 'empty' or 'unpopulated'
//Each cell with three neighbors becomes populated.

//========================================================================
int main( ){

    // Set up the GL context
    ofSetupOpenGL(1024, 768, OF_WINDOW);    // <-------- setup the GL context

    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());

}

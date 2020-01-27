#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // OF settings
    ofBackground(0);
    ofSetFrameRate(60);
    ofNoFill();
    
    // put points into ptf
    for (int i = 0; i < 256; ++i)
    {
        ptf.addPoint(ofVec3f(-512 + i * 4, 100 * sin(i / 10.f), 100 * cos(i / 10.f)));
    }
    
    // make ellipse to draw along the length of the ptf
    mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
    unsigned numVerts = 20;
    float w = 20;
    float h = 40;
    for (unsigned i = 0; i < numVerts; ++i)
    {
        mesh.addVertex(ofVec3f(0.f, w * cos(TWO_PI * i / (float)numVerts), h * sin(TWO_PI * i / (float)numVerts)));
    }
}

//--------------------------------------------------------------
void ofApp::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
}

//--------------------------------------------------------------
void ofApp::draw()
{
    cam.begin();
    
    ofPushMatrix();
    
    // rotate around x axis
    ofRotateDeg(50.f * ofGetElapsedTimef(), 1, 0, 0);
    ofSetColor(0, 255, 255);
    
    for (int i = 0; i < ptf.framesSize(); ++i)
    {
        ofPushMatrix();
        
        // multiply current matrix (rotated around x axis)
        // by transform for next frame
        ofMultMatrix(ptf.frameAt(i));
        
        // draw ellipse
        mesh.draw();
        
        // draw axis
        ofDrawAxis(10);
        
        ofPopMatrix();
    }
    
    ofPopMatrix();
    
    cam.end();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

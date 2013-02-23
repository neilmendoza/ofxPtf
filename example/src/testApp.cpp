#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
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
void testApp::update()
{
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
}

//--------------------------------------------------------------
void testApp::draw()
{
    cam.begin();
    
    ofPushMatrix();
    
    // rotate around y axis
    ofRotate(50.f * ofGetElapsedTimef(), 1, 0, 0);
    ofSetColor(0, 255, 255);
    
    for (int i = 0; i < ptf.framesSize(); ++i)
    {
        ofPushMatrix();
        
        // multiply current matrix (rotated around y axis)
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
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
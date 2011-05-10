#include "testApp.h"

void testApp::setup(){
	ofSetFrameRate(60);  
  
  // set up kinect
	kinect.init();
	kinect.setVerbose(true);
	kinect.open();  
  
  // TODO kinect calibration
  
  // set up surf
  surfImage.allocate(320, 240);  
  surfMotion.setup(&surfImage);
}


void testApp::update(){
	ofBackground(100,100,100);

  
  kinect.update();
  
	if (kinect.isFrameNew()) {
    // pass the RGB image to surf (find a way to get high-res?)
    surfImage.setFromPixels(kinect.getCalibratedRGBPixels(), 320, 240);
    surfMotion.update();
	}

}


void testApp::draw(){
  ofSetColor(255, 255, 255);
  kinect.draw(0, 0, 320, 240);
  surfMotion.draw(0, 0, 1);


}


void testApp::keyPressed(int key){


}


void testApp::keyReleased(int key){

}


void testApp::mouseMoved(int x, int y ){

}


void testApp::mouseDragged(int x, int y, int button){

}


void testApp::mousePressed(int x, int y, int button){

}


void testApp::mouseReleased(int x, int y, int button){

}


void testApp::windowResized(int w, int h){

}


void testApp::gotMessage(ofMessage msg){

}


void testApp::dragEvent(ofDragInfo dragInfo){ 

}

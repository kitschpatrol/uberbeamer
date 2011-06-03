#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
//#include "surflib.h"
//#include "kmeans.h"
//#include "ofxOpenSurf.h"
#include "3d/ofMesh.h"
#include "ofxOsc.h"
#include "wiimote.h"
#include "ofxAutoControlPanel.h"
//#include "ofCamera.h"

class testApp : public ofBaseApp{
  
public:
  void setup();
  void update();
  void draw();
  
  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y );
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);		
  int getFocus(int distance);  
  void drawSphere();
  void focus(); 
  // kinect
  ofxKinect kinect;  
  
  // NO SURF UNTIL WE HAVE RGB CALIBRATION
  // surf
  //    ofxSurfImage surfImage;  
  //    ofxSurfMotion surfMotion;
  //    IpVec surfPoints;
  //    IpPairVec surfMatches;
  
  // control panel
  ofxAutoControlPanel panel;
  
  // mesh
  ofMesh mesh;
  
  
  // point cloud class for mesh alignment???
  // port rbgd reconstructor?
  
  
  // focus distance
	ofSerial	serial;
  
  int kinectFrames;
  // IMU
  wiimote wii;
  float xRotation, yRotation, zRotation;
  
  float zzoom;
  // camera stuff
  ofEasyCam cam;
  
  bool neutralize;
  
  
  float yawTrimAccumulator;
  
  
  
  
};


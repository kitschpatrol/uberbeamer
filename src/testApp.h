#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "surflib.h"
//#include "kmeans.h"
#include "ofxOpenSurf.h"
#include "ofxAutoControlPanel.h"


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

    // kinect
		ofxKinect kinect;  
  
    // surf
    ofxSurfImage surfImage;  
    ofxSurfMotion surfMotion;
    IpVec surfPoints;
    IpPairVec surfMatches;
  
    // control panel
    ofxAutoControlPanel panel;
  
    // mesh
    vector<ofVec3f> meshGrid;  
  
  
    // point cloud class for mesh alignment???
    // port rbgd reconstructor?
  
  
    // focus distance
    // TODO
  
  
    // IMU
    // TODO
    
  

};


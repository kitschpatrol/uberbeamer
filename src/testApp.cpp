#include "testApp.h"

void testApp::setup(){
	ofSetFrameRate(60);  
  
  // set up serial
	serial.enumerateDevices();
	serial.setup("/dev/tty.usbmodemfa141", 9600);
  
  // set up kinect
	kinect.init();  
	kinect.setVerbose(true);
	kinect.open();  
  
  // TODO kinect calibration
  kinectFrames = 0;
  
  // set up surf
  //  surfImage.allocate(320, 240);  
  //  surfMotion.setup(&surfImage);
  
  // set up control panel  
	panel.setup("Control Panel", ofGetWidth() - 315, 5, 300, 600);
	panel.addPanel("camera");
  panel.addSlider("z pos", "zpos", 600, -500, 1000, false);  
  panel.addSlider("x pos", "xroff", 0, -180, 180, false);  
  panel.addSlider("y pos", "yroff", -180, -180, 180, false);  
  panel.addSlider("z pos", "zroff", 0, -180, 180, false);
  panel.addSlider("yaw trim", "yawTrim", 0, -2, 2, false);  
  panel.addToggle("show kinect images", "showKinect", true);  
	panel.addPanel("focus");  
  panel.addToggle("focus connected", "focusConnected", false);
  panel.addToggle("auto focus", "autoFocus", true);
  panel.addSlider("focus", "focus", 50, 27, 80, true);
  
  drawSphere();
  
  
  // set up camera
  //  cam.resetTransform();
  //  cam.clearParent();
  //  //cam.doMouseOrbit = false;
  //	cam.setPosition(0.0, 0.0, 0.0); // eye level is 20 tall?
  //  cam.setFov(60);
  //  cam.setDistance(0);
  
  // not tilt to match kinect
	
  // schedule view neutralization, sets offsets and rotations to 0
  neutralize = true;
  
  yawTrimAccumulator = 0;
}

void testApp::update(){
	ofBackground(0,0,0);  
  kinect.update();
	wii.update();
  
  if (neutralize) {
    cout << "Neutralizing" << endl;
    xRotation = 0;
    yRotation = 0;
    zRotation = 0;
    yawTrimAccumulator = 0;    
    drawSphere(); // reset the sphere
    neutralize = false;
  }
  else {
    // use the wiimote
    yawTrimAccumulator += panel.getValueF("yawTrim");
    
    xRotation = ofMap(wii.pitch, 0, 1, 90, -90);  
    yRotation = ofMap(wii.yawPlus, 0, 1, 0, 360) + yawTrimAccumulator; // TODO fix weird wrap

    zRotation = ofMap(wii.rollPlus, 0, 1, -180, 180);
    // TODO sort out roll
    zRotation = 0;
  }
  
	if (kinect.isFrameNew()) {
    kinectFrames++;
    
    // focus
    if(kinectFrames % 5 == 0) {
      focus();
    }
    
    // distance from the current mesh
    int numExistingVertices = mesh.getNumVertices();
    ofVec3f * existingVertices = mesh.getVerticesPointer();    
    
    //int vertexStep = panel.getValueI("vertexStep");
    int vertexStep = 40;
    
    int xMax = kinect.width - (vertexStep * 1);
    int yMax = kinect.height - (vertexStep * 1);
    
    ofVec3f spherePoint;
    ofVec3f cloudPoint;    
    ofVec3f cloudNormal;
    
    // better to use angle between vectors instead of normals to find matches? function exists. nope.
    
    // todo generate cloud normals
    for (int y = 0; y < yMax; y += vertexStep) {
      for (int x = 0; x < xMax; x += vertexStep) {
        
        cloudPoint = kinect.getWorldCoordinateFor(x, y);
        // make sure we have data
        if (cloudPoint.z > 0) {
          
          // rotate it according to where we're facing
          cloudPoint.rotate(xRotation, ofVec3f(0, 0, 0), ofVec3f(1, 0, 0));            
          cloudPoint.rotate(yRotation, ofVec3f(0, 0, 0), ofVec3f(0, 1, 0));
          cloudPoint.rotate(zRotation, ofVec3f(0, 0, 0), ofVec3f(0, 0, 1));            
          
          // multiply it out, why do we do this?
          cloudPoint *= 100;
          
          cloudNormal = cloudPoint.getNormalized();            
          
          
          // find the closest matching sphere point, then lerp towards the new value (to curb noise)
          
          int minIndex = 0;
          float minDistance = 100000;
          
          for (int i = 0; i < numExistingVertices; i++) {
            spherePoint = existingVertices[i];
            
            float distance = cloudNormal.distance(spherePoint.getNormalized());
            
            
            if (distance < minDistance) {
              minDistance = distance;
              minIndex = i;
            }
            
            // TODO set a cutoff distance, not close enough is not close enough
            
          }
          
          // now lerp towards whatever was closest
          
          // are we pushing or pulling the mesh? only push
          if (existingVertices[minIndex].length() < cloudPoint.length()) {
            existingVertices[minIndex].scale(ofLerp(existingVertices[minIndex].length(),  cloudPoint.length(), 0.2));
          }
        }
      }
    }
    
    
    
  }
  
  
}



void testApp::draw(){
  ofSetColor(255, 255, 255);
  
  ofPushMatrix();

  // center origin
  ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
  
  // target vector, center of point cloud (what if doesn't exist? use most recent?)
  ofVec3f target = kinect.getCalibration().getWorldCoordinateFor(320, 240, 255.0 / 100.0) * 100;//
  
  target.rotate(xRotation, ofVec3f(0, 0, 0), ofVec3f(1, 0, 0));            
  target.rotate(yRotation, ofVec3f(0, 0, 0), ofVec3f(0, 1, 0));
  target.rotate(zRotation, ofVec3f(0, 0, 0), ofVec3f(0, 0, 1));     
  
  //  cam.resetTransform();
  //  cam.setGlobalPosition(0, 0, 0);
  //  cam.setTarget(target);
  //  
  //  cam.begin();  
  
  
  // draw origin lines

  
  ofVec3f origin = ofVec3f(0.0, 0.0, 0.0);
  ofVec3f upX = ofVec3f(100.0, 0.0, 0.0);
  ofVec3f upY = ofVec3f(0.0, 100.0, 0.0);
  ofVec3f upZ = ofVec3f(0.0, 0.0, 100.0);
  
  if(true) {
    
    ofSetColor(255, 0, 0);
    ofLine(origin, upX);
    ofDrawBitmapString("X", 105, 0);    
    
    ofSetColor(0, 255, 0);
    ofLine(origin, upY);
    ofDrawBitmapString("Y", 0, 105);        
    
    ofSetColor(0, 0, 255);
    ofLine(origin, upZ);    
    ofPushMatrix();
    ofTranslate(0.0, 0.0, 105.0);
    ofDrawBitmapString("Z", 0, 0);
    ofPopMatrix();
    
  }
  
  // draw kinect frustum
  
  // show wii rotation
  ofPushMatrix();
  
  ofRotateX(xRotation);
  ofRotateY(yRotation);
  ofRotateZ(zRotation);  
  
  ofVec3f topLeft = kinect.getCalibration().getWorldCoordinateFor(0.0, 0.0, 255.0 / 100.0) * 100;
  ofVec3f topRight = kinect.getCalibration().getWorldCoordinateFor(640.0, 0.0, 255.0 / 100.0) * 100;
  ofVec3f bottomRight = kinect.getCalibration().getWorldCoordinateFor(640.0, 480.0, 255.0 / 100.0) * 100;    
  ofVec3f bottomLeft = kinect.getCalibration().getWorldCoordinateFor(0.0, 480.0, 255.0 / 100.0) * 100;
  
  if(false) {
    
    ofSetColor(255, 0, 0);
    ofLine(topLeft, topRight);
    ofLine(topRight, bottomRight);
    ofLine(bottomRight, bottomLeft);
    ofLine(bottomLeft, topLeft);
    ofLine(origin, topLeft);
    ofLine(origin, topRight);
    ofLine(origin, bottomRight);
    ofLine(origin, bottomLeft);
  }
  
  // draw kinect point cloud
  int xStep = 20;
  int yStep = 20;    
  
  ofSetColor(255, 255, 255);
  glPointSize(2.0);
  glBegin(GL_POINTS);  
  
  for (int x = xStep; x < 640; x += xStep) {
    for (int y = yStep; y < 480; y += yStep) {
      ofPoint cur = kinect.getWorldCoordinateFor(x, y) * 100;
      ofColor color = kinect.getCalibratedColorAt(x,y);
      glColor3ub((unsigned char)color.r,(unsigned char)color.g,(unsigned char)color.b);
      glVertex3f(cur.x, cur.y, cur.z);
    }
  }    
  glEnd();
  
  
  ofPopMatrix();
  
  
  //  // draw the mesh
  //  glEnable(GL_LIGHTING);
  //  glEnable(GL_DEPTH_TEST);
  //  glEnable(GL_LIGHT0);
  //  glEnable(GL_NORMALIZE);  
  //  
	//ofSetColor(100, 100, 100, 100);
	//mesh.drawFaces();
	//ofSetColor(0, 255, 0, 255);
  
  
  //  glDisable(GL_LIGHTING);
  //  glDisable(GL_DEPTH_TEST);    
  
  ofPushMatrix();
  ofTranslate(0, 0, panel.getValueF("zpos"));
  
  
  
  gluLookAt(0, 0, 0, target.x, target.y, target.z, 0, 1, 0);
  // TEMP OFF
//  ofRotateX(xRotation + panel.getValueF("xroff"));
//  ofRotateY(yRotation + panel.getValueF("yroff"));  
//  ofRotateZ(zRotation + panel.getValueF("zroff"));  
  
	ofSetLineWidth(2);
  ofSetColor(255, 255, 255, 255);
  ofEnableSmoothing();
  mesh.drawWireframe();  
  ofDisableSmoothing();
	//mesh.drawVertices();
  
  ofPopMatrix();    
  
  
  
  // back to top left origin for overlay drawing
  ofPopMatrix();  

  
  
  //cam.end();
  
  
  
  // heads up display  
  
  // show IMU angles
  ofNoFill();
  ofSetLineWidth(1);
  ofSetColor(255, 0, 0);  
  
  ofPushMatrix();
  ofTranslate(100, 600);
  
  // Yaw
  ofPushMatrix();
  ofRotate(yRotation);
  ofLine(0, 0, 0, -50);
  ofCircle(0, 0, 50);
  ofPopMatrix();
  
  
  // Pitch
  float p = ofMap(wii.pitch, 0, 1, 50, -50);
  
  ofPushMatrix();
  ofTranslate(70, 0);
  ofRect(-10, -50, 20, 100);
  ofLine(-10, p, 10, p);
  ofPopMatrix();
  
  // Roll
  ofPushMatrix();
  ofTranslate(0, 70);
  ofRotate(zRotation);
  ofLine(0, -10, 0, 10);
  ofLine(-50, 0, 50, 0);
  ofPopMatrix();
  
  ofPopMatrix();  
  
  
  ofSetColor(255, 255, 255);
  
  // draw kinect video feeds
  if (panel.getValueB("showKinect")) {
    kinect.draw(0, 0, 160, 120);
    kinect.drawDepth(160, 0, 160, 120);
  }
}


void testApp::keyPressed(int key){
  if (key == 'f') ofToggleFullscreen();
  if (key == 'n') neutralize = true;
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


void testApp::drawSphere() {
  mesh.clear();
  mesh.setMode(OF_TRIANGLE_STRIP_MODE);
  
  // create the sphere mesh which we will progressively poke outward
  // http://local.wasp.uwa.edu.au/~pbourke/texture_colour/spheremap/  Paul Bourke's sphere code
  // We should weigh an alternative that reduces the batch count by using GL_TRIANGLES instead
  ofVec3f center = ofVec3f(0, 0, 0);
  float radius = 100;
  int segments = 50;
  
  for (int j = 0; j < segments / 2; j++) {
    float theta1 = j * 2 * PI / segments - HALF_PI ;
    float theta2 = (j + 1) * 2 * PI / segments - HALF_PI;
    
    for (int i = 0; i <= segments; i++ ) {
      ofVec3f e, p, pt;
      float theta3 = i * TWO_PI / segments;
      
      e.x = cos(theta1) * cos(theta3);
      e.y = sin(theta1);
      e.z = cos(theta1) * sin(theta3);
      p = e * radius + center;
      
      //normals[i*3*2+0] = e.x; normals[i*3*2+1] = e.y; normals[i*3*2+2] = e.z;
      //  texCoords[i*2*2+0] = 0.999f - i / (float)segments; texCoords[i*2*2+1] = 0.999f - 2 * j / (float)segments;
      
      pt = p;
      
      mesh.addVertex(pt);
      e.x = cos( theta2 ) * cos( theta3 );
      e.y = sin( theta2 );
      e.z = cos( theta2 ) * sin( theta3 );
      p = e * radius + center;
      //normals[i*3*2+3] = e.x; normals[i*3*2+4] = e.y; normals[i*3*2+5] = e.z;
      //texCoords[i*2*2+2] = 0.999f - i / (float)segments; texCoords[i*2*2+3] = 0.999f - 2 * ( j + 1 ) / (float)segments;
      //verts[i*3*2+3] = p.x; verts[i*3*2+4] = p.y; verts[i*3*2+5] = p.z;
      mesh.addVertex(p);
      
      ofColor c = ofColor(ofRandom(0, 1), ofRandom(0, 1), ofRandom(0, 1));
      c.r=1;
      c.g=0;
      c.b=0;
      //mesh.addColor(c);
      //mesh.addColor(c);      
      
    }
    //glDrawArrays( GL_TRIANGLE_STRIP, 0, (segments + 1)*2 );
  }  
}

void testApp::focus() {
  int focusValue;
  
  // TK manual override
  if(panel.getValueB("autoFocus")) {
    // auto focus
    
    unsigned char * depthPixels = kinect.getDepthPixels();        
    
    // find average depth
    int averageDepth = 0;
    int averageCount = 0;
    
    for(int i = 0; i < 640 * 480; i++) {
      if((int)depthPixels[i] > 0) {
        averageDepth += (int)depthPixels[i];
        averageCount++;
      }
    }
    
    averageDepth /= averageCount;        
    
    focusValue = getFocus(averageDepth);    
    
    panel.setValueF("focus", focusValue);
    
  }
  else {
    // manual focus with slider
    focusValue = round(panel.getValueF("focus"));        
  }
  
  // send it to the servo
  // unsigned char focusByte = (unsigned char)focusValue;
  if(panel.getValueB("focusConnected")) {      
    serial.writeByte(focusValue);
  }  
}

int testApp::getFocus(int distance) {
  int focusLength = 13;
  
  int focusTable[13][2] = {
    {162, 27},
    {152, 38},
    {145, 46},
    {140, 52},
    {137, 57},
    {133, 64},
    {132, 66},
    {129, 70},
    {127, 68},
    {125, 71},
    {126, 75},
    {123, 75},
    {122, 80}};
  
  // what about " no data? "  
  
  if (distance >= focusTable[0][0]) {
    // too close
    return focusTable[0][1];
  }
  else if (distance <= focusTable[focusLength - 1][0]) {
    // too far
    return focusTable[focusLength - 1][1];
  }
  else {
    // in range    
    for (int i = 0; i < focusLength - 1; i++) {
      if (distance == focusTable[i][0]) {
        // exact match        
        return focusTable[i][1];
      }
      else if ((distance < focusTable[i][0]) && (distance > focusTable[i + 1][0])) {
        // in between match
        return round(ofMap(distance, focusTable[i][0], focusTable[i + 1][0], focusTable[i][1], focusTable[i + 1][1]));        
      }
    }
    
    // should never get here
    return focusTable[0][1];
  }
}
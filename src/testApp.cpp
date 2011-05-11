#include "testApp.h"

void testApp::setup(){
	ofSetFrameRate(60);  
  
  // set up kinect
	kinect.init();
	kinect.setVerbose(true);
	kinect.open();  
  
  // TODO kinect calibration
  
  
  // set up mesh
  //mesh.setMode(OF_LINE_STRIP_MODE);
  
  
  mesh.setMode(OF_TRIANGLE_STRIP_MODE);

  
  // set up surf
//  surfImage.allocate(320, 240);  
//  surfMotion.setup(&surfImage);
  
  // set up control panel
	panel.setup("Control Panel", ofGetWidth() - 315, 5, 300, 200);
	panel.addPanel("Physics");  
  
  
  // create the sphere mesh which we will progressively poke outward
  // http://local.wasp.uwa.edu.au/~pbourke/texture_colour/spheremap/  Paul Bourke's sphere code
  // We should weigh an alternative that reduces the batch count by using GL_TRIANGLES instead
  
  ofVec3f center = ofVec3f(0, 0, 0);
  float radius = 100;
  int segments = 25;
  
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
      
//        verts[i*3*2+0] = p.x;
//      verts[i*3*2+1] = p.y;
//      verts[i*3*2+2] = p.z;
        
        e.x = cos( theta2 ) * cos( theta3 );
        e.y = sin( theta2 );
        e.z = cos( theta2 ) * sin( theta3 );
        p = e * radius + center;
        //normals[i*3*2+3] = e.x; normals[i*3*2+4] = e.y; normals[i*3*2+5] = e.z;
        //texCoords[i*2*2+2] = 0.999f - i / (float)segments; texCoords[i*2*2+3] = 0.999f - 2 * ( j + 1 ) / (float)segments;
        //verts[i*3*2+3] = p.x; verts[i*3*2+4] = p.y; verts[i*3*2+5] = p.z;
      mesh.addVertex(p);
      }
      //glDrawArrays( GL_TRIANGLE_STRIP, 0, (segments + 1)*2 );
    }
    
    //glDisableClientState( GL_VERTEX_ARRAY );
    //glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    //glDisableClientState( GL_NORMAL_ARRAY );
    
    //delete [] verts;
    //delete [] normals;
    //delete [] texCoords;
  
  
  
}


void testApp::update(){
	ofBackground(0,0,0);  
  kinect.update();
	wii.update();
  xRotation = ofMap(wii.pitch, 0, 1, 180, -180);  
  yRotation = ofMap(wii.yawPlus, 0, 1, -180, 180);
  zRotation = ofMap(wii.rollPlus, 0, 1, -180, 180);

  
	if (kinect.isFrameNew()) {
    // pass the RGB image to surf (find a way to get high-res?)
//    surfImage.setFromPixels(kinect.getCalibratedRGBPixels(), 320, 240);
//    surfMotion.update();
    
    // thin out the point cloud
    
    // draw it into space as a mesh at out current angle
    
    //mesh.addVertex(ofVec3f(x,y,0));
    
    // don't add vertices if there are others nearby
    
    
    
      // distance from the current mesh
      int numExistingVertices = mesh.getNumVertices();
      ofVec3f * existingVertices = mesh.getVerticesPointer();    

      //int vertexStep = panel.getValueI("vertexStep");
      int vertexStep = 40;
  
      int xMax = kinect.width - (vertexStep * 1);
      int yMax = kinect.height - (vertexStep * 1);

      ofVec3f spherePoint;
      ofVec3f sphereNormal;
      ofVec3f cloudPoint;    
      ofVec3f cloudNormal;    
    
      // better to use angle between vectors instead of normals to find matches? function exists.
    
      // todo generate cloud normals
    
    
      for (int y = 0; y < yMax; y += vertexStep) {
        for (int x = 0; x < xMax; x += vertexStep) {
        
          cloudPoint = kinect.getWorldCoordinateFor(x, y) * 100;
          
          // make sure we have data
          if (cloudPoint.z > 0) {
            
            // rotate it according to where we're facing
            cloudPoint.rotate(xRotation, ofVec3f(0, 0, 0), ofVec3f(1, 0, 0));            
            cloudPoint.rotate(yRotation, ofVec3f(0, 0, 0), ofVec3f(0, 1, 0));
            cloudPoint.rotate(zRotation, ofVec3f(0, 0, 0), ofVec3f(0, 0, 1));            
            
            // normalize it
            cloudNormal = cloudPoint.getNormalized();
                        
            // find the closest matching sphere point, then lerp towards the new value (to curb noise)
            
            int minIndex = 0;
            float minDistance = 100000;
            
            for (int i = 0; i < numExistingVertices; i++) {
              spherePoint = existingVertices[i];
              sphereNormal = spherePoint.getNormalized();
              
              float distance = cloudNormal.distance(sphereNormal);

              if (distance < minDistance) {
                minDistance = distance;
                minIndex = i;
              }
            }
            
            // now lerp towards whatever was closest
            // TODO LERP
            existingVertices[minIndex] = cloudPoint;
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
  
  ofRotateY(mouseY);
  ofRotateX(mouseX);
  
  // draw origin lines
  ofVec3f origin = ofVec3f(0.0, 0.0, 0.0);
  ofVec3f upX = ofVec3f(100.0, 0.0, 0.0);
  ofVec3f upY = ofVec3f(0.0, 100.0, 0.0);
  ofVec3f upZ = ofVec3f(0.0, 0.0, 100.0);
  
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

  

  
  
  ofSetColor(255, 0, 0);
  ofLine(topLeft, topRight);
  ofLine(topRight, bottomRight);
  ofLine(bottomRight, bottomLeft);
  ofLine(bottomLeft, topLeft);
  ofLine(origin, topLeft);
  ofLine(origin, topRight);
  ofLine(origin, bottomRight);
  ofLine(origin, bottomLeft);
  
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
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);  
//  
	ofSetColor(100, 100, 100, 100);
	mesh.drawFaces();
	ofSetColor(0, 255, 0, 255);
	//mesh.drawWireframe();
  ofSetColor(255, 255, 255, 255);
	//mesh.drawVertices();
  
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);    
  
  
  
  // back to top left origin for overlay drawing
  ofPopMatrix();  
  
  


  
  

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
  kinect.draw(0, 0, 160, 120);
  kinect.drawDepth(160, 0, 160, 120);
  //  surfMotion.draw(0, 0, 1);  
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

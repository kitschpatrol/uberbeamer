#include "wiimote.h"

wiimote::wiimote(){
	
	debug = false;
	
	receiver.setup(INBOUND_PORT);
	if (debug) printf("Listening for OSC messages via port %i\n", INBOUND_PORT);
	
	sender.setup(HOST, OUTBOUND_PORT);
	if (debug) printf("Sending OSC messages on host %s via port %i\n", HOST, OUTBOUND_PORT);

	pitch = 0;
	roll = 0;
	yaw = 0;
	
	pitchPlus = 0;
	rollPlus = 0;
	yawPlus = 0;	
	
	pointX = 0;
	pointY = 0;
	
	screenX = 0;
	screenY = 0;
	
	lastButton = -2;
	currentButton = -1;
	buttonDown = false;
	buttonReleased = false;
	buttonPressed = false;
	
	distance = 0.5;
	
}


void wiimote::update() {
	
	//cout << "update" << endl;
	
	// check for waiting messages
	while (receiver.hasWaitingMessages()) {
		
		//cout << "got message" << endl;
		
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		
			   if (m.getAddress() == "/wii/1/button/A") m.getArgAsInt32(0) ==  1 ? wiiButtonPressed(WII_BUTTON_A) : wiiButtonReleased(WII_BUTTON_A);
		else if (m.getAddress() == "/wii/1/button/B") m.getArgAsInt32(0) ==  1 ? wiiButtonPressed(WII_BUTTON_B) : wiiButtonReleased(WII_BUTTON_B);
		else if (m.getAddress() == "/wii/1/button/Up") m.getArgAsInt32(0) ==  1 ? wiiButtonPressed(WII_BUTTON_UP) : wiiButtonReleased(WII_BUTTON_UP); 
		else if (m.getAddress() == "/wii/1/button/Down") m.getArgAsInt32(0) ==  1 ? wiiButtonPressed(WII_BUTTON_DOWN) : wiiButtonReleased(WII_BUTTON_DOWN);
		else if (m.getAddress() == "/wii/1/button/Left") m.getArgAsInt32(0) ==  1 ? wiiButtonPressed(WII_BUTTON_LEFT) : wiiButtonReleased(WII_BUTTON_LEFT);
		else if (m.getAddress() == "/wii/1/button/Right") m.getArgAsInt32(0) ==  1 ? wiiButtonPressed(WII_BUTTON_RIGHT) : wiiButtonReleased(WII_BUTTON_RIGHT);
		else if (m.getAddress() == "/wii/1/button/Minus") m.getArgAsInt32(0) ==  1 ? wiiButtonPressed(WII_BUTTON_MINUS) : wiiButtonReleased(WII_BUTTON_MINUS);
		else if (m.getAddress() == "/wii/1/button/Plus") m.getArgAsInt32(0) ==  1 ? wiiButtonPressed(WII_BUTTON_PLUS) : wiiButtonReleased(WII_BUTTON_PLUS);
		else if (m.getAddress() == "/wii/1/button/Home") m.getArgAsInt32(0) ==  1 ? wiiButtonPressed(WII_BUTTON_HOME) : wiiButtonReleased(WII_BUTTON_HOME);
		else if (m.getAddress() == "/wii/1/button/1") m.getArgAsInt32(0) ==  1 ? wiiButtonPressed(WII_BUTTON_ONE) : wiiButtonReleased(WII_BUTTON_ONE);
		else if (m.getAddress() == "/wii/1/button/2") m.getArgAsInt32(0) ==  1 ? wiiButtonPressed(WII_BUTTON_TWO) : wiiButtonReleased(WII_BUTTON_TWO);
		else if (m.getAddress() == "/wii/1/ir") {
			pointX = m.getArgAsFloat(0);
			pointY = m.getArgAsFloat(1);
			
			screenX = ofLerp(0, ofGetWidth(), pointX);
			screenY = ofLerp(ofGetHeight(), 0, pointY);			
			
			if (debug) {
				//printf("IR X: %f  IR Y:  %f\n", pointX, pointY);
				//printf("SCREEN X: %f  SCREEN Y:  %f\n", pointX, pointY);
			}
		}
		else if (m.getAddress() == "/wii/1/accel/pry") {
			pitch = m.getArgAsFloat(0);
			roll = m.getArgAsFloat(1);
			yaw = m.getArgAsFloat(2);

			//if (debug) printf("Pitch: %f  Roll:  %f  Yaw:  %f\n", pitch, roll, yaw);
		}
		else if (m.getAddress() == "/wii/1/motion/angles") {
			pitchPlus = m.getArgAsFloat(0);
			rollPlus = m.getArgAsFloat(1);
			yawPlus = m.getArgAsFloat(2);
			
			// if (debug) printf("Pitch: %f  Roll:  %f  Yaw:  %f\n", pitchPlus, rollPlus, yawPlus);
		}
		else if (m.getAddress() == "/wii/1/ir/xys/1") {
			ir1.x = m.getArgAsFloat(0);
			ir1.y = m.getArgAsFloat(1);
		} 
		else if (m.getAddress() == "/wii/1/ir/xys/2") {
			ir2.x = m.getArgAsFloat(0);
			ir2.y = m.getArgAsFloat(1);
		}
		else if (m.getAddress() == "/wii/1/ir/xys/3") {
			ir3.x = m.getArgAsFloat(0);
			ir3.y = m.getArgAsFloat(1);
		}
		else if (m.getAddress() == "/wii/1/ir/xys/4") {
			ir4.x = m.getArgAsFloat(0);
			ir4.y = m.getArgAsFloat(1);
		}		
		else {
			// unrecognized message
			if (debug) {
				string msg_string;
				msg_string = "Unhandled message: ";				
				msg_string += m.getAddress();
				msg_string += ": ";

				for (int i = 0; i < m.getNumArgs(); i++) {
					// get the argument type
					msg_string += m.getArgTypeName( i );
					msg_string += ":";
					
					// display the argument - make sure we get the right type
					if (m.getArgType(i) == OFXOSC_TYPE_INT32)
						msg_string += ofToString(m.getArgAsInt32(i));
					else if (m.getArgType(i) == OFXOSC_TYPE_FLOAT)
						msg_string += ofToString(m.getArgAsFloat(i));
					else if (m.getArgType(i) == OFXOSC_TYPE_STRING)
						msg_string += m.getArgAsString(i);
					else
						msg_string += "Unknown";
				}

				
				cout << msg_string << endl;	
			}
		}
		
	}
	
	if(currentButton == lastButton) {
		buttonPressed = false;
	}
	if(buttonDown) lastButton = currentButton;
	
	
	// calculate distance
	// only calculate if wiimote isn't at edge
	int irPadding = 200;
	if (((screenX > irPadding) && (screenX < ofGetWidth() - irPadding)) &&  ((screenY > irPadding) && (screenY < ofGetHeight() - irPadding))) {
		// calculate distance (normalizing to max distance of 1...)
		distance = ofLerp(distance, 1 - ofMap(ofDist(ir1.x, ir1.y, ir2.x, ir2.y), 0.0, 1.414213562, 0.0, 1.0), 0.2);	
		//cout << distance << endl;
	}
	
	
}


// WII specific event callbacks, note constants in header file
void wiimote::wiiButtonPressed (int button) {
	currentButton = button;
	buttonDown = true;

	buttonPressed = true;
	buttonReleased = false;
	
	if (debug) printf("Button pressed: %i \n", button);
}

void wiimote::wiiButtonReleased (int button) {
	buttonDown = false;
	buttonReleased = true;
	buttonPressed = false;
	lastButton = -1;
	
	if (debug) printf("Button released: %i \n", button);
}
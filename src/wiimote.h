#ifndef _WIIMOTE
#define _WIIMOTE
#include "ofxOsc.h"
//#define OF_ADDON_USING_OFXOSC
//#include "ofAddons.h"

// FOR USE WITH OSCULATOR

// listen on port 5600, send on localhost
#define HOST "127.0.0.1"
#define INBOUND_PORT 9000
#define OUTBOUND_PORT 9001

// WII button constants
#define WII_BUTTON_A 0
#define WII_BUTTON_B 1
#define WII_BUTTON_UP 2
#define WII_BUTTON_DOWN 3
#define WII_BUTTON_LEFT 4
#define WII_BUTTON_RIGHT 5
#define WII_BUTTON_MINUS 6
#define WII_BUTTON_PLUS 7
#define WII_BUTTON_HOME 8
#define WII_BUTTON_ONE 9
#define WII_BUTTON_TWO 10

class wiimote {
	
public:
	wiimote();
	
	void update();
	void draw();
	
	void wiiButtonPressed(int button);
	void wiiButtonReleased(int button);	

	// orientation
	float pitch;
	float roll;
	float yaw;
	
	float pitchPlus;
	float rollPlus;
	float yawPlus;	

	// x and y
	float pointX;
	float pointY;
	
	float screenX;
	float screenY;
	
	bool buttonDown;
	bool buttonReleased;
	bool buttonPressed;
	int currentButton;

	int lastButton;
	
	
	bool debug;
	
	// raw ir points
	ofPoint ir1;
	ofPoint ir2;
	ofPoint ir3;
	ofPoint ir4;
	
	
	// calculated from IR points
	float distance;
	
	
	
private:
	ofxOscSender sender;
	ofxOscReceiver	receiver;
	ofxOscMessage * mOut;

	
};

#endif	
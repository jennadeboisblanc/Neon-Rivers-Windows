#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "ofxSvg.h"
#include "ofxArtnet.h"
#include "Tributary.h"
//#include "ofxButtons.h"

enum {
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE
};

class ofApp : public ofBaseApp{

	public:

		// built-in to openframeworks
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		// kinect
		ofxKFW2::Device kinect;
		void setupKinect();
		void drawKinect();
		void updateSkeleton();
		void playShow();

		// simulation
		void setupSimulation();
		void updateSimulation();
		void drawSimulation();
		void pulseGradient(int num);
		void drawGlitch(int x, int y, int r, ofColor c);
		void drawTributary(int ind);
		void drawGlitch(int x, int y, int r, ofColor c1, ofColor c2, ofColor c3, ofColor c4);
		void glitchOut();
		void setRandomPulse(int ms, int ps, int sep);
		bool pulsing;
		uint64_t lastChecked;
		int pulseTime;
		int numTracked;
		ofColor gradientColors[9];
		ofColor glitchColors[9];

		// SVG
		ofxSVG svg;
		float step;
		vector<Tributary> tributaries;
		vector<ofPoint> points;

		//DECLARE AN ARTNET NODE
		ofxArtnet anNode;
		//DECLARE DMX DATA = 512 BYTES
		unsigned char dmxData[8][512];
		void setDMXTributaries();

		// button
		/*ButtonManager buttons;
		void initButtons();
		bool bReset;*/
		int numSelect;
		int modeSelect;
		void setMode();
		void changeMode();
		float modeTime;
		int modeDuration;

		// colors
		float storedColors[9][3];
		float storedGlitchColors[4][3];
		void initColors();
		void setStoredColors();
		void setGradientColors();

		// transitioning
		boolean transitioning;
		ofColor previousColors[9];
		void transitionColors();
		void pulseGradientTransition(int num);
		bool checkTransitioning();
		void startTransitioning();
};

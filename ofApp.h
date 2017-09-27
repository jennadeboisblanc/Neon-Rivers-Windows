#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "ofxSvg.h"
#include "ofxArtnet.h"
#include "Tributary.h"

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

		// simulation
		void setupSimulation();
		void updateSimulation();
		void drawSimulation();
		void setRandomPulse(int ms, int ps, int sep);
		bool pulsing;
		uint64_t lastChecked;
		int pulseTime;
		// SVG
		ofxSVG svg;
		float step;
		vector<Tributary> tributaries;
		vector<Tributary> universes;
		vector<ofPoint> points;

		//DECLARE AN ARTNET NODE  
		ofxArtnet anNode;
		//DECLARE DMX DATA = 512 BYTES  
		unsigned char dmxData[512];

};

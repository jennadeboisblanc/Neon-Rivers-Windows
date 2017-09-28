#include "ofApp.h"

int previewWidth = 640;
int previewHeight = 480;

//--------------------------------------------------------------
void ofApp::setup() {
	// Building Data DMX frame  
	// DMX Frame is 512 bytes with value 0 to 255. Here we set every channel to 0.  
	for (int i = 0; i<8; i++) {
		for (int j = 0; j < 512; j++)
		dmxData[i][j] = 0;
	}

	anNode.setup("10.206.231.230");

	setupSimulation();
	// setupKinect();
	// ofSetWindowShape(previewWidth * 2, previewHeight * 2);
}

//--------------------------------------------------------------
void ofApp::update() {
	//setDMXTributaries();
	// updateSkeleton();
	updateSimulation();
	//for (int j = 0; j < 8; j++) {
	//	for (int i = 0; i<512; i++) {
	//		if (i % 3 == 1 && i < 10) {
	//			dmxData[j][i] = 255;
	//		}
	//		else if (i % 3 == 2 && i >= 10) {
	//			dmxData[j][i] = 255;
	//		}
	//		else {
	//			dmxData[j][i] = 0;
	//		}
	//	}
	//}
	
	for (int i = 0; i < 8; i++) {
		anNode.sendDmx("10.206.231.229", 0x0, i, dmxData[i], 512);
	}
		//anNode.sendDmx("10.206.231.229", 0x0, 0x00, dmxData[0], 512);
		//anNode.sendDmx("10.206.231.229", 0x0, 0x01, dmxData2, 512);
		//anNode.sendDmx("10.206.231.229", 0x0, 0x02, dmxData3, 512);
		//anNode.sendDmx("10.206.231.229", 0x0, 0x03, dmxData4, 512);
		//anNode.sendDmx("10.206.231.229", 0x0, 0x04, dmxData5, 512);
		//anNode.sendDmx("10.206.231.229", 0x0, 0x05, dmxData6, 512);
		//anNode.sendDmx("10.206.231.229", 0x0, 0x06, dmxData7, 512);
		//anNode.sendDmx("10.206.231.229", 0x0, 0x07, dmxData8, 512);
}

void ofApp::setDMXTributaries() {
	int unis[8][6] = {
		{ 0, 1, 2, 3 , 4, 40 },		// universe 1
		{ 8, 5, 6, 7, 9, 41 },		// 2
		{12, 13, 10, 11, 14, 42},	// 3
		{17, 18, 19, 15, 16, -1},	// 4
		{22, 23, 24, 20, 21, -1},	// 5
		{25, 26, 29, 30, 31. -1},	// 6
		{27, 28, 37, 38, 39, -1},
		{32, 33, 34, 35, 36, -1}
	};
	for (int u = 0; u < 8; u++) {
		int dx = 0;
		for (int i = 0; i < 6; i++) {
			if (unis[u][i] >= 0) {
				for (int j = 0; j < tributaries[i].pixels.size(); j++) {
					if (dx < 512-3) {
						dmxData[u][dx++] = tributaries[i].pixels.at(j).getRed();
						dmxData[u][dx++] = tributaries[i].pixels.at(j).getGreen();
						dmxData[u][dx++] = tributaries[i].pixels.at(j).getBlue();
					}
				}
			}
		}
	}
}


//--------------------------------------------------------------
void ofApp::draw() {
	// drawKinect();
	drawSimulation();
}

//--------------------------------------------------------------
void ofApp::setupKinect() {
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodySource();
	kinect.initBodyIndexSource();
}

//--------------------------------------------------------------
void ofApp::drawKinect() {}

//--------------------------------------------------------------
void ofApp::updateSkeleton() {
	kinect.update();

	//--
	//Getting joint positions (skeleton tracking)
	//--
	//
	{
		auto bodies = kinect.getBodySource()->getBodies();
		for (auto body : bodies) {
			for (auto joint : body.joints) {
				//now do something with the joints
			}
		}
	}
	//
	//--



	//--
	//Getting bones (connected joints)
	//--
	//
	{
		// Note that for this we need a reference of which joints are connected to each other.
		// We call this the 'boneAtlas', and you can ask for a reference to this atlas whenever you like
		auto bodies = kinect.getBodySource()->getBodies();
		auto boneAtlas = ofxKinectForWindows2::Data::Body::getBonesAtlas();

		for (auto body : bodies) {
			for (auto bone : boneAtlas) {
				auto firstJointInBone = body.joints[bone.first];
				auto secondJointInBone = body.joints[bone.second];

				//now do something with the joints
			}
		}
	}
	//
	//--
}

//--------------------------------------------------------------
void ofApp::setupSimulation() {
	ofSetFrameRate(40);


	ofBackground(255);
	ofSetColor(0);


	// SVG
	vector<ofPolyline> outlines;
	svg.load("install.svg");
	float h = svg.getHeight();
	//float factor = 490/h;
	float factor = (33 * 30.48) / svg.getWidth();
	for (int i = 0; i < svg.getNumPath(); i++) {
		ofPath p = svg.getPathAt(i);
		// svg defaults to non zero winding which doesn't look so good as contours
		p.setPolyWindingMode(OF_POLY_WINDING_ODD);
		vector<ofPolyline>& lines = const_cast<vector<ofPolyline>&>(p.getOutline());
		for (int j = 0;j<(int)lines.size();j++) {
			outlines.push_back(lines[j].getResampledBySpacing(1));
		}
	}
	vector<int> pathOrder = { 39,34,35, 42,28, 29, 9,18, 44,26,27,19,21,8, 41,20, 7,36,10,11, 5, 22, 0, 1, 2,13,15,38,37,14, 12, 25, 3, 4, 17, 23,16,24,6,31,30,43,32,33,40 };

	//svg.getNumPath()
	for (int k = 0; k < pathOrder.size(); k++) {
		ofPath& mypath = svg.getPathAt(pathOrder[k]);
		const vector<ofPolyline>& polylines = mypath.getOutline();
		tributaries.push_back(*new Tributary(k, pathOrder[k], polylines[0], factor));
	}

	tributaries[16].addTributaryEnd(tributaries[17]);
	tributaries[30].addTributaryEnd(tributaries[26]);
	tributaries[30].addTributaryEnd(tributaries[28]);

	int erase[] = { 17, 26, 28 }; //, 29, 30, 31, 32, 37, 40};

	for (int i = 2; i >= 0; i--) {
		tributaries.erase(tributaries.begin() + erase[i]);
	}


	pulsing = true;
	setRandomPulse(40, 2, 30); // millis, packet size, separation

	float totalLEDs = 0;
	float totalCentimeters = 0;
	for (int i = 0; i < tributaries.size(); i++) {
		totalLEDs += tributaries[i].pixels.size();
		totalCentimeters += tributaries[i].len;
	}
	float dmxLEDs = totalLEDs * 3;
	float divideByUniverses = dmxLEDs / 8;

	cout << "total meters: ";
	cout << totalCentimeters / 100 << endl;
	cout << "total LEDs: ";
	cout << totalLEDs << endl;
	cout << "total DMX channels: ";
	cout << dmxLEDs << endl;
	///////////////////////////////////////

}

//--------------------------------------------------------------
void ofApp::updateSimulation() {
	if (pulsing) {
		if (ofGetElapsedTimeMillis() < lastChecked) lastChecked = 0;
		else if (ofGetElapsedTimeMillis() - lastChecked > pulseTime) {
			for (int i = 0; i < tributaries.size(); i++) {
				tributaries[i].updatePulse();
			}
			lastChecked = ofGetElapsedTimeMillis();
		}
	}
}

//--------------------------------------------------------------
void ofApp::drawSimulation() {
	ofSetColor(220);
	ofDrawRectangle(0, 0, 33 * 30.48, 17 * 30.48);
	int erase[] = { 29, 30, 31, 32, 37, 40 };
	for (int i = 0; i < tributaries.size(); i++) {
		//tributaries[i].pulseDraw();
		//tributaries[i].draw(ofColor(255, 255, 0));
		//tributaries[i].drawGradient(ofColor::fromHsb((ofGetElapsedTimeMillis()/100) % 255, 255, 255), ofColor::fromHsb(((ofGetElapsedTimeMillis()/100) + 120) % 255, 255, 255));
		tributaries[i].pulseGradient(ofColor::fromHsb((ofGetElapsedTimeMillis() / 1000) % 255, 255, 255), ofColor::fromHsb(((ofGetElapsedTimeMillis() / 1000) + 120) % 255, 255, 255));

	}
}

//--------------------------------------------------------------
void ofApp::setRandomPulse(int ms, int ps, int sep) {
	pulsing = true;
	pulseTime = ms;
	for (int i = 0; i < tributaries.size(); i++) {
		tributaries[i].setRandomPulse(ps, sep);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	// When key stroke, send the DMX frame (dmxData) which is 512 Bytes long over artnet to the specified IP (Here GrandMA)  
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

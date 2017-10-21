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

	initColors();
	//initButtons();
	

	anNode.setup("10.206.231.230");

	setupSimulation();
	//setupKinect();
	ofSetWindowShape(previewWidth * 2, previewHeight * 2);
	numTracked = 0;
}

//--------------------------------------------------------------
void ofApp::update() {
	//setDMXTributaries();
	//updateSkeleton();
	setMode();
	updateSimulation();
	for (int i = 0; i < 8; i++) {
		anNode.sendDmx("10.206.231.229", 0x0, i, dmxData[i], 512);
	}
}

void ofApp::setDMXTributaries() {
	int decoderUnis[40] = {
		0, 0, 0,
		1, 1, 1,
		2, 2, 2, 2,
		3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4,
		5, 5, 5, 5, 5, 5,
		6, 6, 6, 6, 6,
		7, 7, 7, 7, 7, 7, 7, 7
	};
	int uniIndex[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < 40; i++) {
		int u = decoderUnis[i];
		for (int j = 0; j < tributaries[i].pixels.size(); j++) {
			if (uniIndex[u] < 512 - 3) {
				dmxData[u][uniIndex[u]++] = tributaries[i].pixels.at(j).getRed();
				dmxData[u][uniIndex[u]++] = tributaries[i].pixels.at(j).getGreen();
				dmxData[u][uniIndex[u]++] = tributaries[i].pixels.at(j).getBlue();
			}
		}
	}
	//int unis[8][6] = {
	//	{ 0, 1, 2, -1, -1, -1 },		// universe 1
	//	{3, 4, 5, -1, -1, -1 },		// 2
	//	{6, 7, 8, 9, -1, -1},	// 3
	//	{17, 18, 19, 15, 16, -1},	// 4
	//	{22, 23, 24, 20, 21, -1},	// 5
	//	{25, 26, 29, 30, 31. -1},	// 6
	//	{27, 28, 37, 38, 39, -1},
	//	{32, 33, 34, 35, 36, -1}
	//};
	//for (int u = 0; u < 8; u++) {
	//	int dx = 0;
	//	for (int i = 0; i < 6; i++) {
	//		if (unis[u][i] >= 0) {
	//			for (int j = 0; j < tributaries[i].pixels.size(); j++) {
	//				if (dx < 512-3) {
	//					dmxData[u][dx++] = tributaries[i].pixels.at(j).getRed();
	//					dmxData[u][dx++] = tributaries[i].pixels.at(j).getGreen();
	//					dmxData[u][dx++] = tributaries[i].pixels.at(j).getBlue();
	//				}
	//			}
	//		}
	//	}
	//}
}


//--------------------------------------------------------------
void ofApp::draw() {
	drawSimulation();
	//drawKinect();

}
void ofApp::playShow() {
	//if ()
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
void ofApp::updateSkeleton() {

}

//--------------------------------------------------------------
void ofApp::drawKinect() {
	kinect.update();

	//--
	//Getting joint positions (skeleton tracking)
	//--
	//
	{
		auto bodies = kinect.getBodySource()->getBodies();
		numTracked = 0;
		for (auto body : bodies) {
			if (body.tracked == true) {
				numTracked++;
				for (auto joint : body.joints) {
					//now do something with the joints
					if (joint.first == JointType_HandRight) {
						float x = joint.second.getPosition().x*previewWidth;
						float y = previewHeight - joint.second.getPosition().y*previewHeight;
						//ofCircle(x, y, 50, 50);
						for (int i = 0; i < tributaries.size(); i++) {
							tributaries[i].drawGlitch(x, y, 50, ofColor(255, 0, 0));
						}
					}
					else if (joint.first == JointType_HandLeft) {
						float x = joint.second.getPosition().x*previewWidth;
						float y = previewHeight - joint.second.getPosition().y*previewHeight;
						//ofCircle(x, y, 50, 50);
						for (int i = 0; i < tributaries.size(); i++) {
							tributaries[i].drawGlitch(x, y, 50, ofColor(255, 0, 0));
						}
						//ofCircle(x, y, 50, 50);
					}
					else if (joint.first == JointType_SpineShoulder) {
						float x = joint.second.getPosition().x*previewWidth;
						float y = previewHeight - joint.second.getPosition().y*previewHeight;
						//ofCircle(x, y, 50, 50);
						for (int i = 0; i < tributaries.size(); i++) {
							tributaries[i].drawInRadius(x, y, 100, ofColor(0, 0, 0));
						}
					}
				}
			}
		}
	}
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

	for (int i = 0; i < tributaries.size(); i++) {
		tributaries[i].setGroup(i);
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
	setGradientColors();
	if (ofGetElapsedTimeMillis() < lastChecked) lastChecked = 0;
	if (pulsing) {
		if (ofGetElapsedTimeMillis() - lastChecked > pulseTime) {
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
	//int erase[] = { 29, 30, 31, 32, 37, 40 };
	/*int offset = ofGetElapsedTimeMillis() / 1000;
	ofColor c1 = ofColor::fromHsb(offset % 255, 255, 255);
	ofColor c2 = ofColor::fromHsb((offset + 120) % 255, 255, 255);*/
	if (numTracked < 5) {
		pulseGradient(numSelect);
		//drawGlitch(ofGetMouseX(), ofGetMouseY(), 60, ofColor(0, 0, 0));

		// purple orange glitch
		
		drawGlitch(ofGetMouseX(), ofGetMouseY(), 60, glitchColors[0], glitchColors[1], glitchColors[2], glitchColors[3]);
	}
	else {
		glitchOut();
	}
	//glitchOut();
}

//--------------------------------------------------------------
void ofApp::setRandomPulse(int ms, int ps, int sep) {
	pulsing = true;
	pulseTime = ms;
	for (int i = 0; i < tributaries.size(); i++) {
		tributaries[i].setRandomPulse(ps, sep);
	}
}

void ofApp::pulseGradient(int num) {
	for (int i = 0; i < tributaries.size(); i++) {
		//tributaries[i].pulseDraw();
		//tributaries[i].draw(ofColor(255, 255, 0));
		//tributaries[i].drawGradient(ofColor::fromHsb((ofGetElapsedTimeMillis()/100) % 255, 255, 255), ofColor::fromHsb(((ofGetElapsedTimeMillis()/100) + 120) % 255, 255, 255));
		

		if (num == 2) tributaries[i].pulseGradient(gradientColors[0], gradientColors[1]);
		else if (num == 3) tributaries[i].pulseGradient(gradientColors[0], gradientColors[1], gradientColors[2]);
		else if (num == 4)  tributaries[i].pulseGradient(gradientColors[0], gradientColors[1], gradientColors[2], gradientColors[3]);
		else if (num == 9)  tributaries[i].pulseGradient(gradientColors[0], gradientColors[1], gradientColors[2], gradientColors[3]);


	    
	}
}

void ofApp::drawGlitch(int x, int y, int r, ofColor c) {
	for (int i = 0; i < tributaries.size(); i++) {
		tributaries[i].drawGlitch(x, y, r, c);
	}
}

void ofApp::drawGlitch(int x, int y, int r, ofColor c1, ofColor c2, ofColor c3, ofColor c4) {
	for (int i = 0; i < tributaries.size(); i++) {
		tributaries[i].drawGlitch(x, y, r, c1, c2, c3, c4);
	}
}

void ofApp::glitchOut() {
	for (int i = 0; i < tributaries.size(); i++) {
		tributaries[i].glitchOut();
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
void ofApp::initButtons() {
	numSelect = 9;
	buttons.setup(); // this sets up the events etc..

	ButtonPanel * panel0 = buttons.addButtonPanel("Num Colors  ");
	panel0->addSelectionItem("2", 2, numSelect);
	panel0->addSelectionItem("3", 3, numSelect);
	panel0->addSelectionItem("4", 4, numSelect);
	panel0->addSelectionItem("9", 9, numSelect);

	ButtonPanel * panel1 = buttons.addButtonPanel("  Colors 1-4   ");
	for (int i = 0; i < 4; i++) {
		panel1->addListItem("Color " + to_string(i + 1));
		panel1->addSliderItem("R", 0, 255, storedColors[i][0]);
		panel1->addSliderItem("G", 0, 255, storedColors[i][1]);
		panel1->addSliderItem("B", 0, 255, storedColors[i][2]);
	}
	ButtonPanel * panel2 = buttons.addButtonPanel("  Colors 5-9  ");
	for (int i = 4; i < 9; i++) {
		panel2->addListItem("Color " + to_string(i));
		panel2->addSliderItem("R", 0, 255, storedColors[i][0]);
		panel2->addSliderItem("G", 0, 255, storedColors[i][1]);
		panel2->addSliderItem("B", 0, 255, storedColors[i][2]);
	}
	ButtonPanel * panel3 = buttons.addButtonPanel("  Set Mode  ");
	panel3->addSelectionItem("Icey", 0, modeSelect);
	panel3->addSelectionItem("Rainbow Sherbert", 1, modeSelect);
	panel3->addSelectionItem("Icey Sherbert", 2, modeSelect);
	panel3->addSelectionItem("Green Ice", 3, modeSelect);
	panel3->addSelectionItem("Original 9", 4, modeSelect);

	ButtonPanel * panel4 = buttons.addButtonPanel("  Glitch Colors  ");
	for (int i = 0; i < 4; i++) {
		panel4->addListItem("Color " + to_string(i + 1));
		panel4->addSliderItem("R", 0, 255, storedGlitchColors[i][0]);
		panel4->addSliderItem("G", 0, 255, storedGlitchColors[i][1]);
		panel4->addSliderItem("B", 0, 255, storedGlitchColors[i][2]);
	}
}

void ofApp::setMode() {
	// this mode will last between 45s and 2min
	modeDuration = int(ofRandom(45, 120));
	modeTime = ofGetElapsedTimef();
	if (modeSelect >= 0) {
		
		if (modeSelect == 0) {
			// icey
			numSelect = 4;
			gradientColors[0] = ofColor(153, 255, 255);
			gradientColors[1] = ofColor(153, 204, 255);
			gradientColors[2] = ofColor(204, 255, 229);
			gradientColors[3] = ofColor(204, 255, 255);
			setStoredColors();
		}
		else if (modeSelect == 1) {
			// baskin robbins rainbow sherbert
			numSelect = 4;
			gradientColors[0] = ofColor(255, 204, 204);
			gradientColors[1] = ofColor(255, 178, 102);
			gradientColors[2] = ofColor(255, 102, 178);
			gradientColors[3] = ofColor(127, 0, 255);
			setStoredColors();
		}
		else if (modeSelect == 2) {
			// icey sherbert
			numSelect = 4;
			gradientColors[0] = ofColor(204, 229, 255);
			gradientColors[1] = ofColor(153, 153, 255);
			gradientColors[2] = ofColor(178, 102, 255);
			gradientColors[3] = ofColor(255, 51, 255);
			setStoredColors();
		}
		else if (modeSelect == 3) {
			// green ice
			numSelect = 4;
			gradientColors[0] = ofColor(204, 229, 255);
			gradientColors[1] = ofColor(153, 255, 255);
			gradientColors[2] = ofColor(102, 255, 178);
			gradientColors[3] = ofColor(51, 255, 51);
			setStoredColors();
		}
		else if (modeSelect == 4) {
			numSelect = 9;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			gradientColors[4] = ofColor(204, 229, 255);
			gradientColors[5] = ofColor(153, 255, 255);
			gradientColors[6] = ofColor(102, 255, 178);
			gradientColors[7] = ofColor(51, 255, 51);
			gradientColors[8] = ofColor(128, 255, 0);
			setStoredColors();
		}
		else if (modeSelect == 5) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}
		else if (modeSelect == 6) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}
		else if (modeSelect == 7) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}
		else if (modeSelect == 8) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}
		else if (modeSelect == 9) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}
		else if (modeSelect == 10) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}
		else if (modeSelect == 11) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}
		else if (modeSelect == 12) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}
		else if (modeSelect == 13) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}
		else if (modeSelect == 14) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}
		else if (modeSelect == 15) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}
		else if (modeSelect == 16) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}
		else if (modeSelect == 17) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}
		else if (modeSelect == 18) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}
		else if (modeSelect == 19) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}
		else if (modeSelect == 20) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}
		else if (modeSelect == 21) {
			numSelect = 4;
			gradientColors[0] = ofColor(153, 153, 255);
			gradientColors[1] = ofColor(178, 102, 255);
			gradientColors[2] = ofColor(255, 51, 255);
			gradientColors[3] = ofColor(255, 0, 127);
			setStoredColors();
		}

	}
	modeSelect = -1;
}

void ofApp::changeMode() {
	if (ofGetElapsedTimef() > modeTime + modeDuration) {
	}
}
void ofApp::initColors() {
	gradientColors[0] = ofColor(153, 153, 255);
	gradientColors[1] = ofColor(178, 102, 255);
	gradientColors[2] = ofColor(255, 51, 255);
	gradientColors[3] = ofColor(255, 0, 127);
	gradientColors[4] = ofColor(204, 229, 255);
	gradientColors[5] = ofColor(153, 255, 255);
	gradientColors[6] = ofColor(102, 255, 178);
	gradientColors[7] = ofColor(51, 255, 51);
	gradientColors[8] = ofColor(128, 255, 0);

	glitchColors[0] = ofColor(128, 255, 0);
	glitchColors[1] = ofColor(102, 51, 0);
	glitchColors[2] = ofColor(204, 204, 0);
	glitchColors[3] = ofColor(64, 64, 64);

	setStoredColors();
}

void ofApp::setGradientColors() {
	for (int i = 0; i < 9; i++) {
		gradientColors[i] = ofColor(storedColors[i][0], storedColors[i][1], storedColors[i][2]);
	}
	for (int i = 0; i < 4; i++) {
		glitchColors[i] = ofColor(storedGlitchColors[i][0], storedGlitchColors[i][1], storedGlitchColors[i][2]);
	}
}

void ofApp::setStoredColors() {
	for (int i = 0; i < 9; i++) {
		storedColors[i][0] = gradientColors[i].r;
		storedColors[i][1] = gradientColors[i].g;
		storedColors[i][2] = gradientColors[i].b;
	}
	for (int i = 0; i < 4; i++) {
		storedGlitchColors[i][0] = glitchColors[i].r;
		storedGlitchColors[i][1] = glitchColors[i].g;
		storedGlitchColors[i][2] = glitchColors[i].b;
	}
}
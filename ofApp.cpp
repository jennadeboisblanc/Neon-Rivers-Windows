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

			glitchColors[0] = ofColor(128, 255, 0);
			glitchColors[1] = ofColor(102, 51, 0);
			glitchColors[2] = ofColor(204, 204, 0);
			glitchColors[3] = ofColor(64, 64, 64);
			setStoredColors();
		}
		else if (modeSelect == 1) {
			// baskin robbins rainbow sherbert
			numSelect = 4;
			gradientColors[0] = ofColor(255, 204, 204);
			gradientColors[1] = ofColor(255, 178, 102);
			gradientColors[2] = ofColor(255, 102, 178);
			gradientColors[3] = ofColor(127, 0, 255);

			glitchColors[0] = ofColor(128, 255, 0);
			glitchColors[1] = ofColor(102, 51, 0);
			glitchColors[2] = ofColor(204, 204, 0);
			glitchColors[3] = ofColor(64, 64, 64);
			setStoredColors();
		}
		else if (modeSelect == 2) {
			// icey sherbert
			numSelect = 4;
			gradientColors[0] = ofColor(204, 229, 255);
			gradientColors[1] = ofColor(153, 153, 255);
			gradientColors[2] = ofColor(178, 102, 255);
			gradientColors[3] = ofColor(255, 51, 255);

			glitchColors[0] = ofColor(128, 255, 0);
			glitchColors[1] = ofColor(102, 51, 0);
			glitchColors[2] = ofColor(204, 204, 0);
			glitchColors[3] = ofColor(64, 64, 64);
			setStoredColors();
		}
		else if (modeSelect == 3) {
			// green ice
			numSelect = 4;
			gradientColors[0] = ofColor(204, 229, 255);
			gradientColors[1] = ofColor(153, 255, 255);
			gradientColors[2] = ofColor(102, 255, 178);
			gradientColors[3] = ofColor(51, 255, 51);

			glitchColors[0] = ofColor(128, 255, 0);
			glitchColors[1] = ofColor(102, 51, 0);
			glitchColors[2] = ofColor(204, 204, 0);
			glitchColors[3] = ofColor(64, 64, 64);
			setStoredColors();
		}
		else if (modeSelect == 4) {
			numSelect = 4;
			//hawaiin island
			gradientColors[0] = ofColor(255, 160, 239);
			gradientColors[1] = ofColor(131, 223, 169);
			gradientColors[2] = ofColor(242, 241, 252);
			gradientColors[3] = ofColor(252, 229, 19);

			glitchColors[0] = ofColor(129, 16, 21);
			glitchColors[1] = ofColor(43, 88, 34);
			glitchColors[2] = ofColor(58, 139, 141);
			glitchColors[3] = ofColor(166, 99, 60);
			setStoredColors();
		}
		else if (modeSelect == 5) {
			// this is a new mode
			//green sunlight
			numSelect = 4;
			gradientColors[0] = ofColor(210, 166, 194);
			gradientColors[1] = ofColor(78, 224, 151);
			gradientColors[2] = ofColor(220,255, 255);
			gradientColors[3] = ofColor(253, 252, 42);
			
			glitchColors[0] = ofColor(92, 35,73);
			glitchColors[1] = ofColor(55, 3,179);
			glitchColors[2] = ofColor(123, 114,147);
			glitchColors[3] = ofColor(232, 238, 155);
			setStoredColors();
		}
		else if (modeSelect == 6) {
			numSelect = 4;
			//cherry orange cha cha
			gradientColors[0] = ofColor(246, 239, 235);
			gradientColors[1] = ofColor(252, 171, 219);
			gradientColors[2] = ofColor(176, 241, 209);
			gradientColors[3] = ofColor(251, 218, 199);

			glitchColors[0] = ofColor(66, 61, 173);
			glitchColors[1] = ofColor(69, 191, 100);
			glitchColors[2] = ofColor(42, 61, 33);
			glitchColors[3] = ofColor(116, 36, 142);
			setStoredColors();
		}
		else if (modeSelect == 7) {
			numSelect = 4;
			//electric sherbert
			gradientColors[0] = ofColor(255, 204, 204);
			gradientColors[1] = ofColor(255, 178, 102);
			gradientColors[2] = ofColor(255, 102, 178);
			gradientColors[3] = ofColor(127, 0, 255);

			glitchColors[0] = ofColor(38, 33, 15);
			glitchColors[1] = ofColor(70, 24, 34);
			glitchColors[2] = ofColor(214, 68, 33);
			glitchColors[3] = ofColor(43, 194, 49);
			setStoredColors();
		}
		else if (modeSelect == 8) {
			numSelect = 4;
			//winter cherry blossom 
			gradientColors[0] = ofColor(255, 0,0 );
			gradientColors[1] = ofColor(0, 255, 255);
			gradientColors[2] = ofColor(255, 235, 137);
			gradientColors[3] = ofColor(255, 0, 121);

			glitchColors[0] = ofColor(51, 43, 16);
			glitchColors[1] = ofColor(221, 71,80);
			glitchColors[2] = ofColor(67, 68, 34);
			glitchColors[3] = ofColor(104, 91, 207);
			setStoredColors();
		}
		else if (modeSelect == 9) {
			numSelect = 4;
			//Rasta Hail Selassie
			gradientColors[0] = ofColor(255,255 , 89);
			gradientColors[1] = ofColor(242, 254, 0);
			gradientColors[2] = ofColor(0, 254, 0);
			gradientColors[3] = ofColor(255, 0, 0z);

			glitchColors[0] = ofColor(24, 16, 86);
			glitchColors[1] = ofColor(29, 24, 43);
			glitchColors[2] = ofColor(9, 23, 11);
			glitchColors[3] = ofColor(7, 5, 6);
			setStoredColors();
		}
		else if (modeSelect == 10) {
			numSelect = 4;
			//avacado sky
			gradientColors[0] = ofColor(235, 248, 150);
			gradientColors[1] = ofColor(243, 244, 249);
			gradientColors[2] = ofColor(126, 234, 215);
			gradientColors[3] = ofColor(220, 255, 255);

			glitchColors[0] = ofColor(234, 34, 16);
			glitchColors[1] = ofColor(70, 71, 132);
			glitchColors[2] = ofColor(160, 68, 34);
			glitchColors[3] = ofColor(104, 91, 238);
			setStoredColors();
		}
		else if (modeSelect == 11) {
			numSelect = 4;
			//peaches and cream
			gradientColors[0] = ofColor(255, 178, 183);
			gradientColors[1] = ofColor(252, 255,192);
			gradientColors[2] = ofColor(251, 251, 227);
			gradientColors[3] = ofColor(254, 227, 207);

			glitchColors[0] = ofColor(91, 16, 34);
			glitchColors[1] = ofColor(126, 46,131);
			glitchColors[2] = ofColor(29, 124, 64);
			glitchColors[3] = ofColor(48, 68, 92);
			setStoredColors();
		}
		else if (modeSelect == 12) {
			numSelect = 4;
			//pillowy sky
			gradientColors[0] = ofColor(191, 207, 248);
			gradientColors[1] = ofColor(250, 198, 220);
			gradientColors[2] = ofColor(252, 236, 229);
			gradientColors[3] = ofColor(248, 215, 254);

			glitchColors[0] = ofColor(48, 22, 144);
			glitchColors[1] = ofColor(81, 160, 50);
			glitchColors[2] = ofColor(182, 138, 135);
			glitchColors[3] = ofColor(111, 18, 64);
			setStoredColors();
		}
		else if (modeSelect == 13) {
			numSelect = 3;
			//Ponchatrain Sky
			gradientColors[0] = ofColor(204, 255, 204);
			gradientColors[1] = ofColor(255, 255, 102);
			gradientColors[2] = ofColor(235, 202, 255);
			

			glitchColors[0] = ofColor(82, 108, 197);
			glitchColors[1] = ofColor(197, 114, 192);
			glitchColors[2] = ofColor(231, 270,233);
			glitchColors[3] = ofColor(11, 160, 209);
			setStoredColors();
		}
		else if (modeSelect == 14) {
			numSelect = 4;
			//patriot shmatriot
			gradientColors[0] = ofColor(255, 141, 158);
			gradientColors[1] = ofColor(57, 252, 255);
			gradientColors[2] = ofColor(82, 131, 219);
			gradientColors[3] = ofColor(174, 162, 164 );

			glitchColors[0] = ofColor(19, 84, 43);
			glitchColors[1] = ofColor(40, 12, 25);
			glitchColors[2] = ofColor(181, 113, 58);
			glitchColors[3] = ofColor(207, 40, 60);
			setStoredColors();
		}
		else if (modeSelect == 15) {
			numSelect = 4;
			//gold bayou
			gradientColors[0] = ofColor(255, 202, 118);
			gradientColors[1] = ofColor(179, 255, 219);
			gradientColors[2] = ofColor(88, 156, 168);
			gradientColors[3] = ofColor(255, 245, 255);

			glitchColors[0] = ofColor(155, 164, 144);
			glitchColors[1] = ofColor(131, 54, 179);
			glitchColors[2] = ofColor(182, 25, 25);
			glitchColors[3] = ofColor(154, 126, 64);
			setStoredColors();
		}
		else if (modeSelect == 16) {
			numSelect = 4;
			//playhouse rainbow
			gradientColors[0] = ofColor(255, 208, 255);
			gradientColors[1] = ofColor(255, 255, 247);
			gradientColors[2] = ofColor(100, 231, 242);
			gradientColors[3] = ofColor(255, 240, 175);

			glitchColors[0] = ofColor(251, 244, 86);
			glitchColors[1] = ofColor(29, 6, 43);
			glitchColors[2] = ofColor(88, 16, 16);
			glitchColors[3] = ofColor(96, 11, 34);
			setStoredColors();
		}
		else if (modeSelect == 17) {
			numSelect = 4;
			//fire in the sky
			gradientColors[0] = ofColor(255,255, 55);
			gradientColors[1] = ofColor(255, 65, 67);
			gradientColors[2] = ofColor(234, 235, 255);
			gradientColors[3] = ofColor(255, 61, 250);

			glitchColors[0] = ofColor(34, 245, 86);
			glitchColors[1] = ofColor(29, 6, 43);
			glitchColors[2] = ofColor(88, 16, 16);
			glitchColors[3] = ofColor(96, 11, 34);
			setStoredColors();
		}
		else if (modeSelect == 18) {
			numSelect = 3;
			//smokey merlot
			gradientColors[0] = ofColor(155, 124, 132);
			gradientColors[1] = ofColor(205, 255, 247);
			gradientColors[2] = ofColor(255, 169, 255);
			

			glitchColors[0] = ofColor(255, 19, 0);
			glitchColors[1] = ofColor(55, 30, 179);
			glitchColors[2] = ofColor(18, 114, 2);
			glitchColors[3] = ofColor(232, 0, 6);
			setStoredColors();
		}
		else if (modeSelect == 19) {
			numSelect = 3;
			//hawaiin snowball
			gradientColors[0] = ofColor(241, 184, 233);
			gradientColors[1] = ofColor(153, 179, 207);
			gradientColors[2] = ofColor(224, 234, 39);
			

			glitchColors[0] = ofColor(51, 43, 16);
			glitchColors[1] = ofColor(221, 71, 80);
			glitchColors[2] = ofColor(15, 247, 34);
			glitchColors[3] = ofColor(104, 48, 207);
			setStoredColors();
		}
		else if (modeSelect == 20) {
			numSelect = 4;
			//lime lagoon
			gradientColors[0] = ofColor(66, 255, 250);
			gradientColors[1] = ofColor(240, 229, 249);
			gradientColors[2] = ofColor(221, 255, 182);
			gradientColors[3] = ofColor(78, 195, 231);

			glitchColors[0] = ofColor(33, 17, 75);
			glitchColors[1] = ofColor(101, 193, 156);
			glitchColors[2] = ofColor(36, 48, 173);
			glitchColors[3] = ofColor(78, 61, 22);
			setStoredColors();
		}
		else if (modeSelect == 21) {
			numSelect = 4;
			//icey bayou
			gradientColors[0] = ofColor(0, 221, 255);
			gradientColors[1] = ofColor(255, 255, 0 );
			gradientColors[2] = ofColor(79, 255, 253);
			gradientColors[3] = ofColor(0, 249, 11);

			glitchColors[0] = ofColor(42, 164, 73);
			glitchColors[1] = ofColor(55, 170, 179);
			glitchColors[2] = ofColor(124, 25, 147);
			glitchColors[3] = ofColor(34, 21, 64);
			setStoredColors();
		}
		else if (modeSelect == 22) {
			// fireball
			numSelect = 4;
			gradientColors[0] = ofColor(234, 221, 255);
			gradientColors[1] = ofColor(255, 207, 40);
			gradientColors[2] = ofColor(255, 255, 255);
			gradientColors[3] = ofColor(255, 143, 92);

			glitchColors[0] = ofColor(155, 164, 144);
			glitchColors[1] = ofColor(131, 111, 179);
			glitchColors[2] = ofColor(182, 25, 147);
			glitchColors[3] = ofColor(154, 126, 64);
			setStoredColors();
		}
		else if (modeSelect == 23) {
			// moulin rougue
			numSelect = 4;
			gradientColors[0] = ofColor(231, 120, 255);
			gradientColors[1] = ofColor(167, 104, 150);
			gradientColors[2] = ofColor(102, 86, 178);
			gradientColors[3] = ofColor(241, 188, 253);

			glitchColors[0] = ofColor(66, 61, 173);
			glitchColors[1] = ofColor(69, 191, 100);
			glitchColors[2] = ofColor(42,25, 33);
			glitchColors[3] = ofColor(188, 36, 104);
			setStoredColors();
		}
		else if (modeSelect == 24) {
			// sunset jewel
			numSelect = 4;
			gradientColors[0] = ofColor(255, 231, 204);
			gradientColors[1] = ofColor(255, 178, 102);
			gradientColors[2] = ofColor(255, 102, 178);
			gradientColors[3] = ofColor(0, 181, 255);

			glitchColors[0] = ofColor(61, 129, 99);
			glitchColors[1] = ofColor(69, 189, 34);
			glitchColors[2] = ofColor(194, 178, 33);
			glitchColors[3] = ofColor(86, 194, 209);
			setStoredColors();
		}
		else if (modeSelect == 25) {
			// cherry ice
			numSelect = 4;
			gradientColors[0] = ofColor(254, 22, 131);
			gradientColors[1] = ofColor(255, 103, 232);
			gradientColors[2] = ofColor(255, 229, 255);
			gradientColors[3] = ofColor(225, 48, 158);

			glitchColors[0] = ofColor(155, 164, 5);
			glitchColors[1] = ofColor(131, 111, 179);
			glitchColors[2] = ofColor(182, 25, 147);
			glitchColors[3] = ofColor(154, 126, 64);
			setStoredColors();
		}
		else if (modeSelect == 26) {
			numSelect = 4;
			//dusk over dawn
			gradientColors[0] = ofColor(153, 67, 252);
			gradientColors[1] = ofColor(108, 208, 199);
			gradientColors[2] = ofColor(204, 237, 229);
			gradientColors[3] = ofColor(229, 224, 127);

			glitchColors[0] = ofColor(128, 255, 0);
			glitchColors[1] = ofColor(212, 51, 143);
			glitchColors[2] = ofColor(67, 11, 0);
			glitchColors[3] = ofColor(64, 64, 64);
			setStoredColors();
		}
		else if (modeSelect == 27) {
			//bayou st. john
			numSelect = 4;
			gradientColors[0] = ofColor(204, 229, 255);
			gradientColors[1] = ofColor(153, 255, 255);
			gradientColors[2] = ofColor(102, 255, 178);
			gradientColors[3] = ofColor(51, 255, 51);

			glitchColors[0] = ofColor(61, 128, 98);
			glitchColors[1] = ofColor(69, 189, 34);
			glitchColors[2] = ofColor(193, 178, 33);
			glitchColors[3] = ofColor(85, 194, 209);
			setStoredColors();
		}
		else if (modeSelect == 28) {
			numSelect = 4;
			//peach green tea
			gradientColors[0] = ofColor(28, 240, 178);
			gradientColors[1] = ofColor(242, 218, 70);
			gradientColors[2] = ofColor(252, 236, 175);
			gradientColors[3] = ofColor(252, 200, 61);

			glitchColors[0] = ofColor(91, 108, 34);
			glitchColors[1] = ofColor(40, 46, 131);
			glitchColors[2] = ofColor(162, 124, 143);
			glitchColors[3] = ofColor(48, 118, 92);
			setStoredColors();
		}
		else if (modeSelect == 29) {
			numSelect = 4;
			//yellow pink brick road
			gradientColors[0] = ofColor(255, 0, 215);
			gradientColors[1] = ofColor(0, 255, 255);
			gradientColors[2] = ofColor(255, 0, 255);
			gradientColors[3] = ofColor(255, 255, 0);

			glitchColors[0] = ofColor(93, 35, 73);
			glitchColors[1] = ofColor(55, 4, 179);
			glitchColors[2] = ofColor(124, 114, 147);
			glitchColors[3] = ofColor(232, 238, 155);
			setStoredColors();
		}
		else if (modeSelect == 30) {
			numSelect = 4;
			//goldfinch 
			gradientColors[0] = ofColor(255, 208, 0);
			gradientColors[1] = ofColor(190, 163, 223);
			gradientColors[2] = ofColor(255, 255, 101);
			gradientColors[3] = ofColor(205, 179, 86);

			glitchColors[0] = ofColor(23, 120, 86);
			glitchColors[1] = ofColor(29, 227, 43);
			glitchColors[2] = ofColor(81, 67, 84);
			glitchColors[3] = ofColor(106, 184, 146);
			setStoredColors();
		}
		else if (modeSelect == 31) {
			numSelect = 3;
			//pinkie and greenie
			gradientColors[0] = ofColor(255, 104, 169);
			gradientColors[1] = ofColor(255, 156, 58);
			gradientColors[2] = ofColor(148, 255, 134);
			

			glitchColors[0] = ofColor(223, 158, 234);
			glitchColors[1] = ofColor(144, 67, 240);
			glitchColors[2] = ofColor(15, 247, 252);
			glitchColors[3] = ofColor(224, 244, 208);
			setStoredColors();
		}
		else if (modeSelect == 32) {
			numSelect = 4;
			//rainbow raspberry 
			gradientColors[0] = ofColor(255, 208, 255);
			gradientColors[1] = ofColor(190, 50, 62);
			gradientColors[2] = ofColor(100, 231, 242);
			gradientColors[3] = ofColor(251, 179, 79);

			glitchColors[0] = ofColor(23, 16, 86);
			glitchColors[1] = ofColor(29, 33, 43);
			glitchColors[2] = ofColor(81, 67, 85);
			glitchColors[3] = ofColor(106, 33, 34);
			setStoredColors();
		}
		else if (modeSelect == 33) {
			numSelect = 4;
			//new orleans jazz
			gradientColors[0] = ofColor(147, 255, 183;
			gradientColors[1] = ofColor(242, 255, 192);
			gradientColors[2] = ofColor(139, 231, 211);
			gradientColors[3] = ofColor(155, 125, 207);

			glitchColors[0] = ofColor(19, 54, 34);
			glitchColors[1] = ofColor(40, 12, 25);
			glitchColors[2] = ofColor(63, 113, 144);
			glitchColors[3] = ofColor(207, 39, 228);
			setStoredColors();
		}
		else if (modeSelect == 34) {
			numSelect = 4;
			//icey bayou
			gradientColors[0] = ofColor(237, 255, 196);
			gradientColors[1] = ofColor(43, 252, 255);
			gradientColors[2] = ofColor(82, 231, 158);
			gradientColors[3] = ofColor(174, 245, 164);

			glitchColors[0] = ofColor(19, 84, 105);
			glitchColors[1] = ofColor(40, 12, 25);
			glitchColors[2] = ofColor(63, 113, 58);
			glitchColors[3] = ofColor(207, 40, 60);
			setStoredColors();
		}
		else if (modeSelect == 35) {
			numSelect = 4;
			//incoming twighlight
			gradientColors[0] = ofColor(174, 249, 235);
			gradientColors[1] = ofColor(86, 94, 219);
			gradientColors[2] = ofColor(150, 111, 209);
			gradientColors[2] = ofColor(251, 174, 255);


			glitchColors[0] = ofColor(147, 60, 173);
			glitchColors[1] = ofColor(69, 54, 100);
			glitchColors[2] = ofColor(147, 61, 33);
			glitchColors[3] = ofColor(195, 36, 142);
			setStoredColors();
		}
		else if (modeSelect == 36) {
			numSelect = 4;
			//muted blue raspberry
			gradientColors[0] = ofColor(249, 217, 240);
			gradientColors[1] = ofColor(96, 182, 240);
			gradientColors[2] = ofColor(188, 135, 155);
			gradientColors[3] = ofColor(118, 125, 195);

			glitchColors[0] = ofColor(34, 16, 86);
			glitchColors[1] = ofColor(126, 163, 131);
			glitchColors[2] = ofColor(29, 31, 64);
			glitchColors[3] = ofColor(48, 30, 12);
			setStoredColors();
		}
		else if (modeSelect == 37) {
			numSelect = 4;
			//lemon lime bayou
			gradientColors[0] = ofColor(130, 156, 150);
			gradientColors[1] = ofColor(94, 164, 249);
			gradientColors[2] = ofColor(126, 234, 215);
			gradientColors[3] = ofColor(220, 255, 167);

			glitchColors[0] = ofColor(39, 34, 16);
			glitchColors[1] = ofColor(70, 24, 35);
			glitchColors[2] = ofColor(214, 68, 34);
			glitchColors[3] = ofColor(43, 194, 49);
			setStoredColors();
		}
		else if (modeSelect == 38) {
			numSelect = 4;
			//cool blue bayou
			gradientColors[0] = ofColor(192, 246, 253);
			gradientColors[1] = ofColor(101, 255, 219);
			gradientColors[2] = ofColor(124, 255, 146);
			gradientColors[3] = ofColor(202, 248, 204);

			glitchColors[0] = ofColor(155, 51, 144);
			glitchColors[1] = ofColor(131, 54, 50);
			glitchColors[2] = ofColor(182, 138, 190);
			glitchColors[3] = ofColor(17, 18, 64);
			setStoredColors();
		}
		else if (modeSelect == 39) {
			numSelect = 3;
			//bay yellow
			gradientColors[0] = ofColor(255, 104, 28);
			gradientColors[1] = ofColor(255, 255, 58);
			gradientColors[2] = ofColor(148, 255, 255);
			

			glitchColors[0] = ofColor(223, 158, 234);
			glitchColors[1] = ofColor(237, 237, 240);
			glitchColors[2] = ofColor(255, 247, 251);
			glitchColors[3] = ofColor(251, 246, 244);
			setStoredColors();
		}
		else if (modeSelect == 40) {
			numSelect = 4;
			//icey pinks
			gradientColors[0] = ofColor(241, 205, 228);
			gradientColors[1] = ofColor(249, 255, 247);
			gradientColors[2] = ofColor(255, 222, 255);
			gradientColors[2] = ofColor(255, 250, 255);


			glitchColors[0] = ofColor(42, 164, 73);
			glitchColors[1] = ofColor(55, 110, 179);
			glitchColors[2] = ofColor(39, 25, 147);
			glitchColors[3] = ofColor(34, 126, 64);
			setStoredColors();
		}
		else if (modeSelect == 41) {
			numSelect = 4;
			//lime yellow swamp
			gradientColors[0] = ofColor(91, 120, 137);
			gradientColors[1] = ofColor(149, 214, 76);
			gradientColors[2] = ofColor(252, 255, 229);
			gradientColors[2] = ofColor(229, 255, 127);


			glitchColors[0] = ofColor(48, 255, 47);
			glitchColors[1] = ofColor(212, 51, 50);
			glitchColors[2] = ofColor(67, 11, 136);
			glitchColors[3] = ofColor(20, 18, 64);
			setStoredColors();
		}
		else if (modeSelect == 42) {
			numSelect = 4;
			//pastel skyline
			gradientColors[0] = ofColor(139, 140, 250);
			gradientColors[1] = ofColor(94, 255, 126);
			gradientColors[2] = ofColor(255, 235, 2);
			gradientColors[3] = ofColor(255, 114, 0);

			glitchColors[0] = ofColor(34, 17, 75);
			glitchColors[1] = ofColor(101, 31, 156);
			glitchColors[2] = ofColor(236, 48, 173);
			glitchColors[3] = ofColor(190, 60, 22);
			setStoredColors();
		}
		else if (modeSelect == 43) {
			numSelect = 4;
			//electric dusk
			gradientColors[0] = ofColor(246, 242, 61);
			gradientColors[1] = ofColor(251, 21, 219);
			gradientColors[2] = ofColor(62, 93, 255);
			gradientColors[3] = ofColor(241, 121, 255);

			glitchColors[0] = ofColor(66, 61, 173);
			glitchColors[1] = ofColor(69, 191, 100);
			glitchColors[2] = ofColor(42, 25, 33);
			glitchColors[3] = ofColor(188, 36, 104);
			setStoredColors();
		}
		else if (modeSelect == 44) {
			// patriot shmatriot electric
			numSelect = 4;
			gradientColors[0] = ofColor(255, 255, 250);
			gradientColors[1] = ofColor(71, 128, 220);
			gradientColors[2] = ofColor(252, 48, 146);
			gradientColors[3] = ofColor(15, 215, 254);

			glitchColors[0] = ofColor(48, 181, 144);
			glitchColors[1] = ofColor(81, 212, 50);
			glitchColors[2] = ofColor(182, 138, 136);
			glitchColors[3] = ofColor(17, 18, 64);
			setStoredColors();
		}
		else if (modeSelect == 45) {
			// split compliment sky
			numSelect = 3;
			gradientColors[0] = ofColor(177, 122, 175);
			gradientColors[1] = ofColor(114, 215, 251);
			gradientColors[2] = ofColor(234, 148, 61);
		

			glitchColors[0] = ofColor(255, 255, 0);
			glitchColors[1] = ofColor(55, 233, 179);
			glitchColors[2] = ofColor(233, 114, 147);
			glitchColors[3] = ofColor(232, 238, 255);
			setStoredColors();
		}

		else if (modeSelect == 46) {
			// split compliment sky
			numSelect = 4;
			gradientColors[0] = ofColor(106, 223, 205);
			gradientColors[1] = ofColor(153,133 , 255);
			gradientColors[2] = ofColor(204, 110, 229);
			gradientColors[2] = ofColor(204, 249, 255);

			glitchColors[0] = ofColor(128, 255, 0);
			glitchColors[1] = ofColor(102, 51, 0);
			glitchColors[2] = ofColor(204, 204, 0);
			glitchColors[3] = ofColor(64, 64, 64);
			setStoredColors();
		}
		else if (modeSelect == 47) {
			numSelect = 4;
			//hawaiin island bright
			gradientColors[0] = ofColor(255, 62, 254);
			gradientColors[1] = ofColor(48, 237, 174);
			gradientColors[2] = ofColor(255, 236, 172);
			gradientColors[3] = ofColor(255, 255, 19);

			glitchColors[0] = ofColor(129, 16, 21);
			glitchColors[1] = ofColor(43, 88, 34);
			glitchColors[2] = ofColor(58, 139, 141);
			glitchColors[3] = ofColor(166, 99, 60);
			setStoredColors();
		}
		else if (modeSelect == 48) {
			// this is a new mode
			//orange teal saturated
			numSelect = 4;
			gradientColors[0] = ofColor(246, 63, 235);
			gradientColors[1] = ofColor(56, 171, 218);
			gradientColors[2] = ofColor(65, 255, 255);
			gradientColors[3] = ofColor(255, 244, 199);

			glitchColors[0] = ofColor(92, 35, 73);
			glitchColors[1] = ofColor(55, 3, 179);
			glitchColors[2] = ofColor(123, 114, 147);
			glitchColors[3] = ofColor(232, 238, 155);
			setStoredColors();
		}
		else if (modeSelect == 49) {
			numSelect = 4;
			//saturated green blue yellow
			gradientColors[0] = ofColor(0, 255, 204);
			gradientColors[1] = ofColor(200, 255, 0);
			gradientColors[2] = ofColor(255, 202, 0);
			gradientColors[3] = ofColor(0, 0, 255);

			glitchColors[0] = ofColor(48, 22, 144);
			glitchColors[1] = ofColor(81, 160, 50);
			glitchColors[2] = ofColor(182, 138, 135);
			glitchColors[3] = ofColor(111, 18, 64);
			setStoredColors();
		}
		else if (modeSelect == 50) {
			numSelect = 4;
			//new orleans jazz vibrant
			gradientColors[0] = ofColor(74, 255, 107;
			gradientColors[1] = ofColor(178, 255, 218);
			gradientColors[2] = ofColor(138, 255, 212);
			gradientColors[3] = ofColor(117, 138, 255);

			glitchColors[0] = ofColor(19, 54, 34);
			glitchColors[1] = ofColor(40, 12, 25);
			glitchColors[2] = ofColor(63, 113, 144);
			glitchColors[3] = ofColor(207, 39, 228);
			setStoredColors();
		}
		else if (modeSelect == 51) {
			numSelect = 4;
			//bay yellow pink 
			gradientColors[0] = ofColor(255, 104, 43);
			gradientColors[1] = ofColor(255, 255, 3);
			gradientColors[2] = ofColor(19, 255, 111);
			gradientColors[2] = ofColor(255, 176, 255);


			glitchColors[0] = ofColor(223, 158, 234);
			glitchColors[1] = ofColor(237, 237, 240);
			glitchColors[2] = ofColor(255, 247, 251);
			glitchColors[3] = ofColor(251, 246, 244);
			setStoredColors();
		}
		else if (modeSelect == 52) {
			numSelect = 4;
			//icey bayou electric
			gradientColors[0] = ofColor(0, 255, 255);
			gradientColors[1] = ofColor(15, 5, 255);
			gradientColors[2] = ofColor(255, 255, 253);
			gradientColors[3] = ofColor(0, 172, 255);

			glitchColors[0] = ofColor(42, 164, 73);
			glitchColors[1] = ofColor(55, 170, 179);
			glitchColors[2] = ofColor(124, 25, 147);
			glitchColors[3] = ofColor(34, 21, 64);
			setStoredColors();
		}
		else if (modeSelect == 53) {
			numSelect = 4;
			//orange skyline
			gradientColors[0] = ofColor(255, 57, 61);
			gradientColors[1] = ofColor(255, 91, 255);
			gradientColors[2] = ofColor(255, 255, 78);
			gradientColors[3] = ofColor(255, 172, 19);

			glitchColors[0] = ofColor(42, 164, 73);
			glitchColors[1] = ofColor(55, 170, 179);
			glitchColors[2] = ofColor(124, 25, 147);
			glitchColors[3] = ofColor(34, 21, 64);
			setStoredColors();
	}
		else if (modeSelect == 54) {
			numSelect = 4;
			//orange skyline
			gradientColors[0] = ofColor(0, 0, 255);
			gradientColors[1] = ofColor(255, 137, 0);
			gradientColors[2] = ofColor(0, 255, 255);
			gradientColors[3] = ofColor(255, 0, 255);

			glitchColors[0] = ofColor(42, 255, 73);
			glitchColors[1] = ofColor(255, 252, 0);
			glitchColors[2] = ofColor(255, 25, 147);
			glitchColors[3] = ofColor(34, 21, 255);
			setStoredColors();
		}
		else if (modeSelect == 55) {
			numSelect = 4;
			//deep blue green
			gradientColors[0] = ofColor(91, 120, 137);
			gradientColors[1] = ofColor(94, 255, 126);
			gradientColors[2] = ofColor(255, 235, 2);
			gradientColors[2] = ofColor(255, 114, 0);


			glitchColors[0] = ofColor(48, 255, 47);
			glitchColors[1] = ofColor(212, 51, 50);
			glitchColors[2] = ofColor(67, 11, 136);
			glitchColors[3] = ofColor(20, 18, 64);
			setStoredColors();
		}
		else if (modeSelect == 55) {
			numSelect = 4;
			//deep blue green
			gradientColors[0] = ofColor(91, 120, 137);
			gradientColors[1] = ofColor(94, 72, 126);
			gradientColors[2] = ofColor(255, 255, 2);
			gradientColors[2] = ofColor(255, 114, 0);


			glitchColors[0] = ofColor(48, 255, 47);
			glitchColors[1] = ofColor(212, 51, 50);
			glitchColors[2] = ofColor(67, 11, 136);
			glitchColors[3] = ofColor(20, 18, 64);
			setStoredColors();
		}
		else if (modeSelect == 56) {
			numSelect = 4;
			//pink blue white
			gradientColors[0] = ofColor(255, 252, 255);
			gradientColors[1] = ofColor(255, 8, 96);
			gradientColors[2] = ofColor(0, 0, 255);
			gradientColors[2] = ofColor(255, 0, 220);


			glitchColors[0] = ofColor(48, 255, 47);
			glitchColors[1] = ofColor(212, 51, 50);
			glitchColors[2] = ofColor(67, 11, 136);
			glitchColors[3] = ofColor(20, 18, 64);
			setStoredColors();
		}
		else if (modeSelect == 57) {
			numSelect = 4;
			//orange bayou
			gradientColors[0] = ofColor(255, 183, 255);
			gradientColors[1] = ofColor(4, 170, 243);
			gradientColors[2] = ofColor(0, 255, 255);
			gradientColors[2] = ofColor(255, 204, 7);


			glitchColors[0] = ofColor(48, 255, 47);
			glitchColors[1] = ofColor(212, 51, 50);
			glitchColors[2] = ofColor(67, 11, 136);
			glitchColors[3] = ofColor(20, 18, 64);
			setStoredColors();
		}
		else if (modeSelect == 58) {
			numSelect = 4;
			//yellow green blue
			gradientColors[0] = ofColor(150, 255, 255);
			gradientColors[1] = ofColor(5, 164, 243);
			gradientColors[2] = ofColor(0, 255, 255);
			gradientColors[2] = ofColor(201, 255, 69);


			glitchColors[0] = ofColor(126, 103, 13);
			glitchColors[1] = ofColor(212, 54, 48);
			glitchColors[2] = ofColor(255, 12, 40);
			glitchColors[3] = ofColor(94, 54, 29);
			setStoredColors();
		}
		else if (modeSelect == 59) {
			numSelect = 4;
			//the blues
			gradientColors[0] = ofColor(168, 0, 255);
			gradientColors[1] = ofColor(0, 197, 243);
			gradientColors[2] = ofColor(0, 255, 255);
			gradientColors[2] = ofColor(0, 0, 255);


			glitchColors[0] = ofColor(48, 255, 47);
			glitchColors[1] = ofColor(212, 51, 50);
			glitchColors[2] = ofColor(67, 11, 136);
			glitchColors[3] = ofColor(20, 18, 64);
			setStoredColors();
		}
		else if (modeSelect == 60) {
			numSelect = 4;
			//the greens
			gradientColors[0] = ofColor(222, 255, 101);
			gradientColors[1] = ofColor(191, 255, 1);
			gradientColors[2] = ofColor(0, 255, 229);
			gradientColors[2] = ofColor(87, 255, 255);


			glitchColors[0] = ofColor(137, 0, 159);
			glitchColors[1] = ofColor(211, 2, 0);
			glitchColors[2] = ofColor(255, 12, 0);
			glitchColors[3] = ofColor(94, 54, 0);
			setStoredColors();
		}
		else if (modeSelect == 61) {
			numSelect = 4;
			//reds
			gradientColors[0] = ofColor(255, 0, 0);
			gradientColors[1] = ofColor(255, 134, 0);
			gradientColors[2] = ofColor(255, 36, 122);
			gradientColors[2] = ofColor(255, 200, 0);


			glitchColors[0] = ofColor(10, 0, 255);
			glitchColors[1] = ofColor(212, 2, 255);
			glitchColors[2] = ofColor(32, 12, 133);
			glitchColors[3] = ofColor(94, 54, 0);
			setStoredColors();
		}
		else if (modeSelect == 62) {
			numSelect = 4;
			//yellow stars
			gradientColors[0] = ofColor(255, 255, 0);
			gradientColors[1] = ofColor(255, 223, 0);
			gradientColors[2] = ofColor(255, 160, 255);
			gradientColors[2] = ofColor(255, 242, 226);


			glitchColors[0] = ofColor(30, 255, 159);
			glitchColors[1] = ofColor(38, 255, 255);
			glitchColors[2] = ofColor(34, 255, 133);
			glitchColors[3] = ofColor(45, 255, 0);
			setStoredColors();
		}
		else if (modeSelect == 63) {
			numSelect = 4;
			//greens again
			gradientColors[0] = ofColor(0, 175, 255);
			gradientColors[1] = ofColor(10, 255, 15);
			gradientColors[2] = ofColor(255, 248, 255);
			gradientColors[2] = ofColor(10, 176, 103);


			glitchColors[0] = ofColor(30, 10, 159);
			glitchColors[1] = ofColor(255, 46, 81);
			glitchColors[2] = ofColor(255, 255, 133);
			glitchColors[3] = ofColor(45, 24, 37);
			setStoredColors();
		}
}

void ofApp::changeMode() {
	if (ofGetElapsedTimef() < modeTime) 
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
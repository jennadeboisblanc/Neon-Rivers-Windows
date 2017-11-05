//
//  tributary.cpp
//  ledInstall
//
//  Created by Jenna deBoisblanc on 6/30/17.
//
//

#include <stdio.h>
#include "Tributary.h"

Tributary::Tributary(){}

Tributary::Tributary(int i, int k, ofPolyline p, float numInStrip){
    id = i;
    originalOrder = k;
    repeat = 1;
    speed = 1;
    packetSize = 1;
    pulseIndex = 0;
	group = 0;
    setup(p, numInStrip);
	lastChecked = ofGetElapsedTimeMillis();
	glitchColor = ofColor(0, 0, 0);
  transitioning = false;
}

void Tributary::setup(ofPolyline p, float numInStrip) {
    //////////////
    // make sure there are the correct number of pixels on the length of the tributary
    len = numInStrip;
    int diodesPerPixel = 7;
	pixelsOnLine = len;
    diodesOnLine = pixelsOnLine * diodesPerPixel;
    totalPoints = diodesOnLine;


    for (int j = 0; j < totalPoints; j+=7){
        vector<ofPoint> diodes;
        for (int i = 0; i < 7; i++) {
            // add a point based on the index/ percent of the way through/
            ofPoint vert = p.getPointAtPercent(1.0*(j+i)/(totalPoints-1));
            vert.set(vert.x*2, vert.y*2);
            diodes.push_back(vert);
        }
        Pixel p = Pixel(diodes);
        pixels.push_back(p);
    }

    //////////////

}

void Tributary::addTributaryEnd(Tributary t) {
    len += t.len;
    totalPoints = int(len * .56);

    for (int i = 0; i < t.pixels.size(); i++) {
        pixels.push_back(t.pixels[i]);
    }
}

void Tributary::setGroup(int g) {
	group = g;
}

//void Tributary::addTributary(int ledsFromEnd, vector <ofPoint> p) {
//    if (p.size()-ledsFromEnd)
//
//}

void Tributary::setRandomPulse(int ps, int separation){
    repeat = packetSize+separation;
    packetSize = ps;
    pulseIndex = int(ofRandom(repeat)+1);
}

void Tributary::update(){
    pulseIndex+=speed;
    if (pulseIndex >= repeat) pulseIndex = 0;

}

void Tributary::updatePulse(){
    pulseIndex++;
    if (pulseIndex >= repeat) pulseIndex = 0;

}


void Tributary::draw(){
    for( int j = 0; j < pixels.size(); j++ ) {
        pixels[j].draw(ofColor::fromHsb(255*(id*1.0/42), 255*(j*1.0/pixels.size()), 255));
    }
}

void Tributary::draw(ofColor c){
    for( int j = 0; j < pixels.size(); j++ ) {
        pixels[j].draw(c);
    }
	drawLabel(pixels[pixels.size()-1].diodes[0]);
}

void Tributary::drawGradient(ofColor start, ofColor end) {
	for (int j = 0; j < pixels.size(); j++) {
		pixels[j].draw(start.lerp(end, j*1.0/pixels.size()));
	}
}

void Tributary::setGradientTransition() {
	transitioning = true;
	lastOffset = 0;
	iterateNum = 0;
	startOffset = 0;
}


void Tributary::updateGradientPulse() {
	if (transitioning) {
		// was += 1 but had to slow down for now
		startOffset += ofGetElapsedTimeMillis() % 2;
		if (startOffset >= pixels.size()) transitioning = false;
	}
	offset += ofGetElapsedTimeMillis()%2;
}

void Tributary::pulseGradient(int num, ofColor prevGradients[], ofColor gradients[]) {
	updateGradientPulse();
	if (transitioning) {
		for (int j = 0; j < pixels.size(); j++) {
			pixels[j].draw(getTransitionGradient(j, num, prevGradients, gradients));
		}
	}
	else {
		for (int j = 0; j < pixels.size(); j++) {
			pixels[j].draw(getWrapGradient(j - (offset%pixels.size()), pixels.size(), num, gradients));
		}
	}
}

/*void Tributary::pulseGradientY(int h, int num, ofColor gradients[]) {
	for (int j = 0; j < pixels.size(); j++) {
		pixels[j].draw(getWrapGradient(pixels[j].getY() - offset, h, num, gradients[]));
	}
}*/

ofColor Tributary::getWrapGradient(int ind, int totalInd, int num, ofColor gradients[]) {
	if (ind > totalInd) ind -= totalInd;
	else if (ind < 0) ind += totalInd;
	for (int i = 0; i < num; i++) {
		if (ind < totalInd/ (num*1.0) * (i+1)) {
			float pos = ofMap(ind, i * totalInd / (num*1.0), (i+1) * totalInd / (num*1.0), 0, 1.0);
			if (i == num - 1) {
				ofColor c = gradients[i];
				return c.lerp(gradients[0], pos);
			}
			else {
				ofColor c = gradients[i];
				return c.lerp(gradients[i+1], pos);
			}
		}
	}
	return ofColor(0);

}

ofColor Tributary::getTransitionGradient(int ind, int num,  ofColor prevGradients[], ofColor gradients[]) {
	if (ind > startOffset) {
		return getWrapGradient(ind - (offset%pixels.size()), pixels.size(), num, prevGradients);
		//return ofColor(0, 0, 255);
	}
	else {
		//return ofColor(255, 0, 0);
		return getWrapGradient(ind - (offset%pixels.size()), pixels.size(), num, gradients);
	}
}


void Tributary::pulseDraw(){
    ofPoint person;
    ofColor c;
    person.set(100, 100);
    for( int j = 0; j < pixels.size(); j++ ) {
         //if (inRadius(j, person, 50)) c = ofColor(255, 0, 0);
         if (inPulse(j)) c = ofColor(0, 255, 0);
         else c = ofColor(ofColor::fromHsb(255*(id*1.0/42), 255*(j*1.0/pixels.size()), 255));

        pixels[j].draw(c);
     }
}

bool Tributary::inPulse(int j) {

    ofPoint vert = pixels[j].diodes[3];

    if ((j-pulseIndex)%repeat >= 0 && (j-pulseIndex)%repeat < packetSize) return true;
    else return false;
}

void Tributary::drawInRadius(float x, float y, float r, ofColor c) {
	for (int j = 0; j < pixels.size(); j++) {
		float dis = pixels[j].getDistance(x, y);
		if (dis < r) {
			//float likelihood = ofMap(dis, 0, r, 1, 0);

			pixels[j].draw(c);
		}
	}
}


void Tributary::drawGlitch(float x, float y, float r, bool change, ofColor c1, ofColor c2, ofColor c3, ofColor c4) {
	for (int j = 0; j < pixels.size(); j++) {
		float dis = pixels[j].getDistance(x, y);
		if (dis < r) {
			//if (change) {
				int r = int(ofRandom(4));
				if (r == 0) pixels[j].setGlitchColor(c1);
				else if (r == 1) pixels[j].setGlitchColor(c2);
				else if (r == 2) pixels[j].setGlitchColor(c3);
				else if (r == 3) pixels[j].setGlitchColor(c4);
			//}
			pixels[j].drawGlitch();
		}
	}
}

void Tributary::drawGroup() {
	for (int j = 0; j < pixels.size(); j++) {
		pixels[j].draw(ofColor::fromHsb((group*10) % 255, 255, 255));
	}
}

void Tributary::glitchOut() {
	ofColor gcs[6] = { ofColor(150), ofColor(255), ofColor(0), ofColor(255, 0, 0), ofColor(255, 255, 0), ofColor(0, 255, 255) };
	for (int j = 0; j < pixels.size(); j++) {
		int ind = int(ofRandom(6));
		pixels[j].draw(gcs[ind]);
		//pixels[j].draw(ofColor(ofRandom(255)));
	}
}
//void Tributary::getData() {
//
//}


//
void Tributary::drawLabel(ofPoint vert) {
    ofSetColor(0);
   // ofDrawBitmapString(originalOrder, vert);
    ofDrawBitmapString(id, vert);

    //ofDrawBitmapString(pixels.size(), vert);
    //ofDrawBitmapString(len*.39, vert);
}

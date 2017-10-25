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
  startOffset = (ofGetElapsedTimeMillis() / 100) % pixels.size();
}

void Tributary::pulseGradientTransition(int num, ofColor[] gradients) {
	int offset = (ofGetElapsedTimeMillis() / 100) % pixels.size();
  if (offset - startOffset >= pixels.size()) transitioning = false;
	else {
    for (int j = 0; j < pixels.size(); j++) {
		    pixels[j].draw(getWrapGradient(j-offset, pixels.size(), num, gradients));
	     }
     }
}

void Tributary::pulseGradient(int num, ofColor[] gradients) {
	int offset = (ofGetElapsedTimeMillis() / 100) % pixels.size();

	for (int j = 0; j < pixels.size(); j++) {
		pixels[j].draw(getWrapGradient(j-offset, pixels.size(), num, gradients));
	}
}

void Tributary::pulseGradient(ofColor start, ofColor end) {
	int offset = (ofGetElapsedTimeMillis() / 100) % pixels.size();

	for (int j = 0; j < pixels.size(); j++) {
		pixels[j].draw(get2WrapGradient(j-offset, pixels.size(), start, end));
	}
}

void Tributary::pulseGradientY(int h, ofColor start, ofColor end) {
	int offset = (ofGetElapsedTimeMillis() / 100) % h;

	for (int j = 0; j < pixels.size(); j++) {
		pixels[j].draw(get2WrapGradient(pixels[j].getY() - offset, h, start, end));
	}
}

void Tributary::pulseGradient(ofColor start, ofColor mid, ofColor end) {
	int offset = (ofGetElapsedTimeMillis() / 100) % pixels.size();

	for (int j = 0; j < pixels.size(); j++) {
		pixels[j].draw(get3WrapGradient(j - offset, pixels.size(), start, mid, end));
	}
}

void Tributary::pulseGradient(ofColor start, ofColor mid1, ofColor mid2, ofColor end) {
	int offset = (ofGetElapsedTimeMillis() / 60) % pixels.size();

	for (int j = 0; j < pixels.size(); j++) {
		pixels[j].draw(get4WrapGradient(j - offset, pixels.size(), start, mid1, mid2, end));
	}
}

void Tributary::pulseGradient(ofColor start, ofColor mid1, ofColor mid2, ofColor mid3, ofColor mid4, ofColor mid5, ofColor mid6, ofColor mid7, ofColor end) {
	int offset = (ofGetElapsedTimeMillis() / 100) % pixels.size();

	for (int j = 0; j < pixels.size(); j++) {
		pixels[j].draw(get9WrapGradient(j - offset, pixels.size(), start, mid1, mid2, mid3, mid4, mid5, mid6, mid7, end));
	}
}

ofColor Tributary::getWrapGradient(int ind, int totalInd, int num, ofColor[] gradients) {
	if (ind > totalInd) ind -= totalInd;
	else if (ind < 0) ind += totalInd;
  for (var i = 0; i < num; i++) {
    if (ind < totalInd/ (num*1.0) * i) {
      float pos = ofMap(ind, i * totalInd / (num*1.0), (i+1) * totalInd / (num*1.0), 0, 1.0);
      if (i == num - 1) {
        return gradients[i].lerp(gradients[0], pos);
      }
      else {
        return gradients[i].lerp(gradients[i+1], pos);
      }
    }
  }
}

ofColor Tributary::get2WrapGradient(int ind, int totalInd, ofColor start, ofColor end) {
	if (ind > totalInd) ind -= totalInd;
	else if (ind < 0) ind += totalInd;
	if (ind > totalInd / 2) {
		float pos = ofMap(ind, totalInd / 2, totalInd, 0, 1.0);
		return end.lerp(start, pos);
	}
	else {
		float pos = ofMap(ind, 0, totalInd / 2, 0, 1.0);
		return start.lerp(end, pos);
	}
}

ofColor Tributary::get3WrapGradient(int ind, int totalInd, ofColor start, ofColor mid, ofColor end) {
	if (ind >= totalInd) ind -= totalInd;
	else if (ind < 0) ind += totalInd;
	if (ind < totalInd / 3.0) {
		float pos = ofMap(ind, 0, totalInd / 3, 0, 1.0);
		return start.lerp(mid, pos);
	}
	else if (ind < totalInd *2.0 / 3) {
		float pos = ofMap(ind, totalInd/3.0, totalInd*2.0/3, 0, 1.0);
		return mid.lerp(end, pos);
	}
	else {
		float pos = ofMap(ind, totalInd*2.0/3, totalInd, 0, 1.0);
		return end.lerp(start, pos);
	}
}

ofColor Tributary::get4WrapGradient(int ind, int totalInd, ofColor start, ofColor mid1, ofColor mid2, ofColor end) {
	if (ind >= totalInd) ind -= totalInd;
	else if (ind < 0) ind += totalInd;
	if (ind < totalInd / 4.0) {
		float pos = ofMap(ind, 0, totalInd / 4.0, 0, 1.0);
		return start.lerp(mid1, pos);
	}
	else if (ind < totalInd / 2.0) {
		float pos = ofMap(ind, totalInd / 4.0, totalInd/2.0, 0, 1.0);
		return mid1.lerp(mid2, pos);
	}
	else if (ind < totalInd *3.0 / 4) {
		float pos = ofMap(ind, totalInd / 2.0, totalInd*3.0 / 4, 0, 1.0);
		return mid2.lerp(end, pos);
	}
	else {
		float pos = ofMap(ind, totalInd*3.0 / 4, totalInd, 0, 1.0);
		return end.lerp(start, pos);
	}
}

ofColor Tributary::get9WrapGradient(int ind, int totalInd, ofColor start, ofColor mid1, ofColor mid2, ofColor mid3, ofColor mid4, ofColor mid5, ofColor mid6, ofColor mid7, ofColor end) {
	if (ind >= totalInd) ind -= totalInd;
	else if (ind < 0) ind += totalInd;
	if (ind < totalInd / 9.0) {
		float pos = ofMap(ind, 0, totalInd / 9.0, 0, 1.0);
		return start.lerp(mid1, pos);
	}
	else if (ind < totalInd * 2.0 / 9.0) {
		float pos = ofMap(ind, totalInd / 9.0, totalInd * 2.0 / 9.0, 0, 1.0);
		return mid1.lerp(mid2, pos);
	}
	else if (ind < totalInd *3.0 / 9.0) {
		float pos = ofMap(ind, totalInd * 2.0/9.0, totalInd*3.0 /9, 0, 1.0);
		return mid2.lerp(mid3, pos);
	}
	else if (ind < totalInd *4.0 / 9.0) {
		float pos = ofMap(ind, totalInd * 3.0 / 9.0, totalInd*4.0 / 9, 0, 1.0);
		return mid3.lerp(mid4, pos);
	}
	else if (ind < totalInd *5.0 / 9.0) {
		float pos = ofMap(ind, totalInd * 4.0 / 9.0, totalInd*5.0 / 9, 0, 1.0);
		return mid4.lerp(mid5, pos);
	}
	else if (ind < totalInd *6.0 / 9.0) {
		float pos = ofMap(ind, totalInd * 5.0 / 9.0, totalInd*6.0 / 9, 0, 1.0);
		return mid5.lerp(mid6, pos);
	}
	else if (ind < totalInd *7.0 / 9.0) {
		float pos = ofMap(ind, totalInd * 6.0 / 9.0, totalInd*7.0 / 9, 0, 1.0);
		return mid6.lerp(mid7, pos);
	}
	else if (ind < totalInd *8.0 / 9.0) {
		float pos = ofMap(ind, totalInd * 7.0 / 9.0, totalInd*8.0 / 9, 0, 1.0);
		return mid7.lerp(end, pos);
	}
	else {
		float pos = ofMap(ind, totalInd*8.0 / 9, totalInd, 0, 1.0);
		return end.lerp(start, pos);
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

void Tributary::drawGlitch(float x, float y, float r, ofColor c) {
	for (int j = 0; j < pixels.size(); j++) {
		float dis = pixels[j].getDistance(x, y);
		if (dis < r) {
			int likelihood = int(ofRandom(0, dis));
		//	if (likelihood < dis/2) {
				if (ofGetElapsedTimeMillis() - lastChecked > 50) {
					//pixels[j].setGlitchColor( ofColor::fromHsb((int(c.getHue() + 10 + ofRandom(20))) % 255, 255, 255));
					pixels[j].setGlitchColor(ofColor(25));
				}
				pixels[j].drawGlitch();
				//pixels[j].draw(ofColor(255, 0, 0));
		//	}

		}
	}
	if (ofGetElapsedTimeMillis() - lastChecked > 50) {
		lastChecked = ofGetElapsedTimeMillis();
	}
	else if (lastChecked > ofGetElapsedTimeMillis()) lastChecked = ofGetElapsedTimeMillis();
}

void Tributary::drawGlitch(float x, float y, float r, ofColor c1, ofColor c2, ofColor c3, ofColor c4) {
	for (int j = 0; j < pixels.size(); j++) {
		float dis = pixels[j].getDistance(x, y);
		if (dis < r) {
			int likelihood = int(ofRandom(0, dis));
			if (ofGetElapsedTimeMillis() - lastChecked > 50) {
				int r = int(ofRandom(4));
				if (r == 0) pixels[j].setGlitchColor(c1);
				else if (r == 1) pixels[j].setGlitchColor(c2);
				else if (r == 2) pixels[j].setGlitchColor(c3);
				else if (r == 3) pixels[j].setGlitchColor(c4);
			}
			pixels[j].drawGlitch();
		}
	}
	if (ofGetElapsedTimeMillis() - lastChecked > 50) {
		lastChecked = ofGetElapsedTimeMillis();
	}
	else if (lastChecked > ofGetElapsedTimeMillis()) lastChecked = ofGetElapsedTimeMillis();
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

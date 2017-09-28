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

Tributary::Tributary(int i, int k, ofPolyline p, float factor){
    id = i;
    originalOrder = k;
    repeat = 1;
    speed = 1;
    packetSize = 1;
    pulseIndex = 0;
    setup(p, factor);
}

void Tributary::setup(ofPolyline p, float factor) {
    //////////////
    // make sure there are the correct number of pixels on the length of the tributary
    len = p.getPerimeter()*factor;  // get perimeter into cm (each pixel = 1 cm)
    int diodesPerPixel = 7;
    float pixelsPerCentimeter = 1/12.5;     // 1 pixel every 12.5 cm => 8 pixels/meter
    pixelsOnLine = ceil(len*pixelsPerCentimeter);
    diodesOnLine = pixelsOnLine * diodesPerPixel;
    
    
    totalPoints = diodesOnLine;
    

    for (int j = 0; j < totalPoints; j+=7){
        vector<ofPoint> diodes;
        for (int i = 0; i < 7; i++) {
            // add a point based on the index/ percent of the way through/
            ofPoint vert = p.getPointAtPercent(1.0*(j+i)/(totalPoints-1));
            vert.set(vert.x*factor, vert.y*factor);
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
}

void Tributary::drawGradient(ofColor start, ofColor end) {
	for (int j = 0; j < pixels.size(); j++) {
		pixels[j].draw(start.lerp(end, j*1.0/pixels.size()));
	}
}

void Tributary::pulseGradient(ofColor start, ofColor end) {
	int offset = (ofGetElapsedTimeMillis() / 100) % pixels.size();

	for (int j = 0; j < pixels.size(); j++) {
		pixels[j].draw(get2WrapGradient(j-offset, pixels.size(), start, end));
	}
}

void Tributary::pulseGradient(ofColor start, ofColor mid, ofColor end) {
	int offset = (ofGetElapsedTimeMillis() / 100) % pixels.size();

	for (int j = 0; j < pixels.size(); j++) {
		pixels[j].draw(get2WrapGradient(j - offset, pixels.size(), start, end));
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
	if (ind > totalInd) ind -= totalInd;
	else if (ind < 0) ind += totalInd;
	if (ind > totalInd / 3) {
		float pos = ofMap(ind, totalInd / 2, totalInd, 0, 1.0);
		return start.lerp(mid, pos);
	}
	else if (ind > totalInd *2.0 / 3) {
		float pos = ofMap(ind, 0, totalInd / 3, 0, 1.0);
		return mid.lerp(end, pos);
	}
	else {
		float pos = ofMap(ind, 0, totalInd / 3, 0, 1.0);
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
		if (pixels[j].inRadius(x, y, r)) pixels[j].draw(c);
	}
}

//void Tributary::setPixel(int index, ofColor c) {
//    
//}

//void Tributary::getData() {
//    
//}


//
//void Tributary::drawLabel(ofPoint vert) {
//    ofSetColor(0);
//   // ofDrawBitmapString(originalOrder, vert);
////    ofDrawBitmapString(id, vert);
//
//    //ofDrawBitmapString(points.size(), vert);
//    ofDrawBitmapString(len*.39, vert);
//}

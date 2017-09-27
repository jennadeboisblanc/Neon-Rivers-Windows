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

bool Tributary::inRadius(int j, ofPoint person, float r) {
    ofPoint p = pixels[j].diodes[3];
    float x = p.x - person.x;
    float y = p.y - person.y;
    float d = sqrt(x*x + y*y);
    if (d < r) {
        return true;
    }
    return false;
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

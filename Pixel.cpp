//
//  Pixel.cpp
//  ledInstall
//
//  Created by Jenna deBoisblanc on 7/6/17.
//
//

#include "Pixel.h"
#include <stdio.h>

Pixel::Pixel(){}

Pixel::Pixel(vector<ofPoint> p){
    diodes = p;
}

void Pixel::draw(ofColor c){
    ofSetColor(c);
    for (int i = 0; i < diodes.size(); i++) {
        ofPoint vert = diodes[i];
        ofDrawEllipse(vert, 5, 5);
    }
}

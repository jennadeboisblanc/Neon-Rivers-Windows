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
	c = 0;
	glitchColor = ofColor(0, 0, 0);
}

void Pixel::draw(ofColor c){
    ofSetColor(c);
	this->c = c;
    for (int i = 0; i < diodes.size(); i++) {
        ofPoint vert = diodes[i];
        ofDrawEllipse(vert, 5, 5);
    }
}

void Pixel::drawGlitch() {
	draw(glitchColor);
}

void Pixel::setColor(ofColor col) {
	c = col;
}

unsigned char Pixel::getRed() {
	return c.r;
}

unsigned char Pixel::getGreen() {
	return c.g;
}

unsigned char Pixel::getBlue() {
	return c.b;
}

bool Pixel::inRadius(float x2, float y2, float r) {
	return getDistance(x2, y2) < r;
}

void Pixel::setGlitchColor(ofColor c) {
	glitchColor = c;
}

int Pixel::getX() {
	return diodes[diodes.size() / 2].x;
}
int Pixel::getY() {
	return diodes[diodes.size() / 2].y;
}

float Pixel::getDistance(float x2, float y2) {
	float x1 = diodes[0].x;
	float y1 = diodes[0].y;
	float dx = (x2 - x1)*(x2 - x1);
	float dy = (y2 - y1) * (y2 - y1);
	return sqrt(dx + dy);
} 
//
//  Pixel.h
//  ledInstall
//
//  Created by Jenna deBoisblanc on 7/6/17.
//
//

#ifndef Pixel_h
#define Pixel_h

#include <stdio.h>
#include "ofMain.h"

class Pixel {
    
public:
    
    void draw(ofColor c);
	void drawGlitch();
	void setColor(ofColor c);
	void setGlitchColor(ofColor c);

    vector<ofPoint> diodes;
    
    Pixel();
    Pixel(vector <ofPoint> diodes);
	ofColor c;
	ofColor glitchColor;

	bool inRadius(float x, float y, float rad);

	unsigned char getRed();
	unsigned char getGreen();
	unsigned char getBlue();

	float getDistance(float x, float y);

	int getX();
	int getY();
    
    
private:
    
};


#endif /* Pixel_h */

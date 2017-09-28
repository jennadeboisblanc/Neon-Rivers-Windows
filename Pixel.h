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
	void setColor(ofColor c);

    vector<ofPoint> diodes;
    
    Pixel();
    Pixel(vector <ofPoint> diodes);
	ofColor c;

	bool inRadius(float x, float y, float rad);

	unsigned char getRed();
	unsigned char getGreen();
	unsigned char getBlue();
    
    
private:
    
};


#endif /* Pixel_h */

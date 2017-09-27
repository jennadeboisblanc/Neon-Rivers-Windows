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
    
    vector<ofPoint> diodes;
    
    Pixel();
    Pixel(vector <ofPoint> diodes);
    
    
private:
    
};


#endif /* Pixel_h */

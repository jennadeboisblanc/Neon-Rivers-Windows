//
//  tributary.h
//  ledInstall
//
//  Created by Jenna deBoisblanc on 6/30/17.
//
//

#ifndef Tributary_h
#define Tributary_h

#include "ofMain.h"
#include "Pixel.h"

class Tributary {
    
public:
    
    void setup(ofPolyline p, float factor);
    void update();
    void draw();
    void draw(ofColor c);
    void drawPixel(int index, ofColor c);
    void drawLabel(ofPoint vert);
    
    void addTributaryEnd(Tributary t);
    
    void setRandomPulse(int packetSize, int separation);
    void pulseDraw();
    void updatePulse();
    
    bool inPulse(int index);
    bool inRadius(int index, ofPoint person, float r);
    
    vector<ofPoint> getPoints();
    

    int id;
    int repeat;
    int speed;
    int packetSize;
    int pulseIndex;
    int totalPoints;
    int pixelsOnLine;
    int diodesOnLine;
    float len;
    int originalOrder;
    
    vector<Pixel> pixels;
    const unsigned char *data512;
    
    
    Tributary();
    Tributary(int i, int orig, ofPolyline p, float f);
    

private:
    
};

#endif /* Tributary_h */

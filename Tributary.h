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
	void setGroup(int g);
    void pulseDraw();
    void updatePulse();
	void drawGroup();
	void drawGradient(ofColor s, ofColor e);
	void pulseGradient(ofColor s, ofColor e);
	void pulseGradient(ofColor s, ofColor m, ofColor e);
	void pulseGradient(ofColor s, ofColor m1, ofColor m2, ofColor e);
	void pulseGradient(ofColor s, ofColor m1, ofColor m2, ofColor m3, ofColor m4, ofColor m5, ofColor m6, ofColor m7, ofColor e);
	void pulseGradientY(int h, ofColor s, ofColor e);
	void glitchOut();
	ofColor get2WrapGradient(int ind, int totalInd, ofColor start, ofColor end);
	ofColor get3WrapGradient(int ind, int totalInd, ofColor start, ofColor mid, ofColor end);
	ofColor get4WrapGradient(int ind, int totalInd, ofColor start, ofColor mid1, ofColor mid2, ofColor end);
	ofColor get9WrapGradient(int ind, int totalInd, ofColor start, ofColor mid1, ofColor mid2, ofColor mid3, ofColor mid4, ofColor mid5, ofColor mid6, ofColor mid7, ofColor end);

	void drawInRadius(float x, float y, float r, ofColor c);
	void drawGlitch(float x, float y, float r, ofColor c);
	void drawGlitch(float x, float y, float r, ofColor c1, ofColor c2, ofColor c3, ofColor c4);

    bool inPulse(int index);
    bool inRadius(int index, ofPoint person, float r);

   // vector<ofPoint> getPoints();

	unsigned long lastChecked;
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
	int group;
	ofColor glitchColor;

    vector<Pixel> pixels;
    const unsigned char *data512;

    // transitioning
    boolean transitioning;
    int startOffset;
    ofColor getWrapGradient(int ind, int totalInd, int num, ofColor[] gradients);
    void pulseGradient(int num, ofColor[] gradients);


    Tributary();
    Tributary(int i, int orig, ofPolyline p, float f);


private:

};

#endif /* Tributary_h */

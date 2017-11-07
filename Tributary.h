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
	void glitchOut();

	void drawInRadius(float x, float y, float r, ofColor c);
	void drawGlitch(float x, float y, float r, bool change, ofColor c1, ofColor c2, ofColor c3, ofColor c4);

    bool inPulse(int index);

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
    bool transitioning;
    int startOffset;
	int iterateNum;
	int lastOffset;
	int offset;
    ofColor getWrapGradient(int ind, int totalInd, int num, ofColor gradients[]);
    void pulseGradient(int num, ofColor prevGradients[], ofColor gradients[]);
	void setGradientTransition();
	void updateGradientPulse();
	ofColor getTransitionGradient(int ind, int num, ofColor gradients[], ofColor prevGradients[]);

    Tributary();
    Tributary(int i, int orig, ofPolyline p, float f);


private:

};

#endif /* Tributary_h */

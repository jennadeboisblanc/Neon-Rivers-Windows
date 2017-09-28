//
//  Universe.h
//  ledInstall
//
//  Created by Jenna deBoisblanc on 7/2/17.
//
//

#ifndef Universe_h
#define Universe_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxArtnet.h"
#include "Tributary.h"


class Universe {
    
public:
    
    void setup();
    void update();
    /*void draw();
    
    int id;
    const unsigned char *data512;
    const unsigned char getData();
    */
    
    Universe(int address);
	int address;
	//DECLARE DMX DATA = 512 BYTES  
	//unsigned char dmxData[512];
	//void sendDMX();

	//vector<Tributary> tributaries;
	
	//static ofxArtnet anNode;
	//static void initArtNet();
    
    
private:
    
};

#endif /* Universe_h */

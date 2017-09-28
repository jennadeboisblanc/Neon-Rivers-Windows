//
//  Universe.cpp
//  ledInstall
//
//  Created by Jenna deBoisblanc on 7/2/17.
//
//

#include "Universe.h"


Universe::Universe(int add){
	address = add;
	for (int i = 0; i<512; i++) {
		//dmxData[i] = 0;
	}
}


void Universe::setup() {
    
}

void Universe::update() {
    
}

//void Universe::sendDMX() {
//	anNode.sendDmx("10.206.231.229", 0x0, address, dmxData, 512);
//}

//void Universe::initArtNet() {
//	anNode.setup("10.206.231.230");
//}

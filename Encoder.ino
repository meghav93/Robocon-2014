#include "Declarations.h"

bool encoder::initialized = false;
encoder *encoder::encList[maxEncoders];
int encoder::numEnc;


inline encoder::encoder(PinNum p1,PinNum p2) : Encoder(p1,p2 > 0 ? p2 : p1 + 1) 
{
	if (!initialized) initClass();
	encList[numEnc] = this;
	numEnc++;
}

inline void encoder::initClass() {
	numEnc = 0;
	for (int i = 0; i< maxEncoders; i++)
		encList[i] = 0;
	initialized = true;
}

inline void encoder::updateAll() {
	for (int i = 0; i<maxEncoders; i++)
		if (encList[i]) encList[i]->read();
}


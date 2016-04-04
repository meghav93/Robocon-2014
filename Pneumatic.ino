#include "Declarations.h"

Pneumatic::Pneumatic(PinNum pin1,int actTime) : ActTime(actTime),P(pin1)
{
	state = Open;
	pinMode(pin1,OUTPUT);
	pinMode(pin1+1,OUTPUT);
	reset();
}

void Pneumatic::reset()
{

}
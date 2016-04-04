#include "Declarations.h"

IR::IR(PinNum P):Pin(P)
{
	pinMode(P,INPUT);
}

bool IR::state()
{
	return digitalRead(Pin);
}
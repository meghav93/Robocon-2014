#include "Declarations.h"

Electromagnet::Electromagnet(PinNum Pin) : P(Pin)
{
	pinMode(P, OUTPUT);
	demagnetize();
}

void Electromagnet::magnetize()
{
	digitalWrite(P, HIGH);
}

void Electromagnet::demagnetize()
{
	digitalWrite(P, LOW);
}
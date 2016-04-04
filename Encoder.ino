#include "Declarations.h"

Encoder::Encoder(PinNum P):Pin(P)
{
    pinMode(Pin,INPUT);
    //attachInterrupt(Pin,interrupt,RISING);
    count=0;
}
void Encoder::interrupt()
{
  count++;
}

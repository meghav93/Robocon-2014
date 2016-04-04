#include "Declarations.h"
#define call(x) if(events.x) events.x();else

Encoder E1(51), E2(52), E3(54);
IR ir1(47),ir2(45),ir3(43);

typedef	void (*func)();

struct Events
{
	func enc1,enc2,enc3,ir1L,ir1R,ir2L,ir2R,ir3L,ir3R,
		rid1,rid2;
	Events()
	{
		enc1=enc2=enc3=ir1L=ir1R=ir2L=ir2R=ir3L=ir3R=
			rid1=rid2=0;
	}

}events;

void intE1()
{
	if(events.enc1) E1.interrupt();
	call(enc1);
}

void intE2()
{
	if(events.enc2) E2.interrupt();
	call(enc2);
}
void intE3()
{
	if(events.enc3) E3.interrupt();
	call(enc3);
}


void conSetup()
{
	attachInterrupt(E1.Pin,intE1,RISING);
	attachInterrupt(E2.Pin,intE2,RISING);
	attachInterrupt(E3.Pin,intE3,RISING);

	/*attachInterrupt(ir1.Pin,intIr1,RISING);
	attachInterrupt(ir2.Pin,intIr2,RISING);
	attachInterrupt(ir3.Pin,intIr3,RISING);*/
}




void conLoop()
{


}


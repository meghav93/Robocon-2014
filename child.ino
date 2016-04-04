#include "Declarations.h"

Child::Child():stp(400, 34, 32, 30, 28, 14),fan(7),EM1(2),EM2(4),EM3(6),E(A0)
{
	stp.setSpeed(30);

}

void Child::pole1()
{
	EM1.magnetize();
	stp.initialize();
	delay(1000);
	fan.setSpeed(FanSpeed);
	while(!digitalRead(IR1))
	{
		fan.update();
	}
	attachInterrupt(A0,interrupt1,RISING);
	E.count=0;
	Serial.println("pole detected");
	while(E.count<1)//wait
	{
		fan.update();
	}
	fan.stop();
	detachInterrupt(A0);
	EM2.magnetize();
	delay(100);
	EM1.demagnetize();
	Serial.println("Stop");
	
}

void Child::pole2()
{
}

void Child::pole3()
{
}

void Child::pole4()
{
}

void Child::poleWalk()
{

}

#include "Declarations.h"

Fan::Fan(PinNum Pin) : P(Pin)
{
	pinMode(P, OUTPUT);

	speed = targetSpeed = 1100;
	

	/*for (int aTime = 0; aTime < 500; aTime++)
	{
		digitalWrite(P, HIGH);
		delayMicroseconds(1100);
		digitalWrite(P, LOW);
		delay(19);
	}*/
}

void Fan::setSpeed(unsigned int Speed)
{
	targetSpeed = Speed;
	lastTime = millis();
}
void Fan::stop()
{
   setSpeed(1100);
   update();
}

void Fan::update()
{
	if (targetSpeed != speed)
	{
		if (targetSpeed < speed) speed -= changeSpeed; else speed += changeSpeed;
	}
	if (speed == 0) return;
	if (millis() - lastTime < 20 - (speed / 1000)) return;

	digitalWrite(P, HIGH);
	delayMicroseconds(speed);
	digitalWrite(P, LOW);
	lastTime = millis();
}

#include "Declarations.h"
#include "Arduino.h"

stepper::stepper(int numSteps, int p1, int p2, int p3, int p4, int limit_switch) : motor(numSteps, p1, p2, p3, p4)
{
        limitPin=limit_switch;
	pinMode(limitPin,INPUT_PULLUP);
        digitalWrite(limitPin,HIGH);
        steps = 0;
	this->numSteps = numSteps;
	lastTime = 0;
	step_delay = 0;
}

void stepper::setSteps(int Steps)
{
	steps = Steps;
}

void stepper::update()
{
	if (steps == 0) return;
	if (millis() - lastTime < step_delay) return;
	if (steps > 0) motor.step(1); else motor.step(-1);
	if (steps > 0) steps--; else steps++;                      //?????
}

void stepper::setSpeed(long whatSpeed)
{
	this->step_delay = 60L * 1000L / this->numSteps / whatSpeed;
	motor.setSpeed(whatSpeed);
}

void stepper::rotateBy(int angle)
{
	setSteps(angle * numSteps / 360.0);
}
void stepper::rotateImmediate(int angle)
{
        motor.step(angle * numSteps /360.0);
}
void stepper::initialize()
{
      
      while(digitalRead(limitPin))
      {
        setSteps(1);
        update();
      }
      rotateBy(EXTRA_ANGLE);

}

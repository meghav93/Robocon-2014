#include "Declarations.h"

Stepper::Stepper(PinNum firstPin, float StepAngle,int delayBetweenPulses,bool halfStep) : stepAngle(StepAngle),waitTime(delayBetweenPulses),P(firstPin),HalfStep(halfStep)
{
	pinMode(P,OUTPUT);
	pinMode(P+1,OUTPUT);
	pinMode(P+2,OUTPUT);
	pinMode(P+3,OUTPUT);
	reset();
}

void Stepper::reset()
{
	highPin = P;
	freewheel();
	angle = targetAngle = 0;
}

void Stepper::setAngle(float newAngle)
{
	targetAngle = newAngle;
	update();
}

void Stepper::step(Direction rotDir)
{
	if (HalfStep)
	{
		//1000 1100 0100 0110 0010 0011 0001 1001
		static bool halfWay = false;

		if (rotDir == CW)
		{
			if (!halfWay)
			{
				low(highPin);
				highPin++;
				if (highPin > P + 3) highPin = P;
			} else
			{
				if (highPin < P + 3) 
					high(highPin + 1);
				else
					high(P);
			}
			angle += stepAngle/2;
		} else
		{
			if (!halfWay)
			{
				low(highPin);
				highPin--;
				if (highPin < P) highPin = P + 3;
			} else
			{
				if (highPin > P) 
					high(highPin - 1);
				else
					high(P);
			}
			angle -= stepAngle/2;
		}
		halfWay = !halfWay;
	}
	else
	{
		if (rotDir == CW) 
		{
			if (highPin > P) low(highPin - 1); else low(P + 3);
			if (highPin < P+3) highPin++; else highPin = P;
			high(highPin);
			angle += stepAngle;
		}
		else
		{
			if (highPin < P+3) low(highPin + 1); else low(P);
			if (highPin > P) highPin--; else highPin = P+3;
			high(highPin);
			angle -= stepAngle;
		}
	}
}

void Stepper::update()
{
	if (abs(targetAngle-angle) < stepAngle) return;
	if (angle > targetAngle)
		step(CW);
	if (angle < targetAngle)
		step(CCW);
}

void Stepper::freewheel()
{
	low(P);
	low(P+1);
	low(P+2);
	low(P+3);
}
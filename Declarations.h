#ifndef DECL_H
#define DECL_H

#include <Arduino.h>
#include <Stepper.h>

#define PinNum char
#define changeSpeed 10
#define EXTRA_ANGLE -8
#define IR1 A7

int FanSpeed = 1500;

class Fan
{
	const PinNum P;
	unsigned speed, targetSpeed;
	long lastTime;
public:
	Fan(PinNum Pin);
	void setSpeed(unsigned int speed);
	void update();
	void stop();
};

class stepper {
	Stepper motor;
	int steps;
	int numSteps;
	int step_delay;
	int lastTime;
	int limitPin;
public:
	stepper(int numSteps, int p1, int p2, int p3, int p4 ,int limit_switch);
	void setSteps(int Steps);
	void rotateBy(int angle);
	void rotateImmediate(int angle);
	void setSpeed(long rpm);
	void initialize();
	void update();
};

class MagSensor
{

};

class Electromagnet
{
	const PinNum P;
public:
	Electromagnet(PinNum Pin);
	void magnetize();
	void demagnetize();
};

class Accelerometer
{
	const PinNum Pin1, Pin2, Pin3;
public:
	Accelerometer(PinNum P1, PinNum P2, PinNum P3);

	void update();
};
class Encoder
{
public:
	const PinNum Pin;
	volatile int count;
	Encoder(PinNum P);
	int read();
	void interrupt();
};

class IR
{
public:
	const PinNum Pin;
	IR(PinNum P);
	bool state();
};

class Child
{
public:
	stepper stp;
	MagSensor m;
	Electromagnet EM1, EM2, EM3;
	Fan fan;
	Encoder E;

	Child();
	void poleWalk();
	void pole1();
	void pole2();
	void pole3();
	void pole4();
	void swing();
	void jungleGym();
	void seesaw();

	void readMsg();
	void grabFloor();
} child;

void interrupt1(){
	child.E.interrupt();
}

#endif

#include "Declarations.h"
#define AccDist 1000.0
#define PIDDist 1440.0
#define MaxSpeed 200.0

bool PIDMotor::initialized = false;
int PIDMotor::nMotors = 0;
PIDMotor *PIDMotor::motorList[maxMotors];


inline PIDMotor::PIDMotor(PinNum Pin1, PinNum Pin2, encoder *e , float diameter) : P1(Pin1), P2(Pin2)
{
	dia = diameter;
	if (!initialized) initClass();
	motorList[nMotors++] = this; 
	pinMode(P1, OUTPUT);
	pinMode(P2, OUTPUT);
	connectedEncoder = e;
	masterMotor = 0;
	slave = false;
	kp = 0.5;
	ki = 0;
	kd = 0.2;
	reset();
}

inline void PIDMotor::reset()
{
	stop();
	accError = 0;
	totalError = 0;
	totalCount = 0;
	work = true;
}

inline void PIDMotor::move(float distInCent, bool start)
{
	work = start;
	if (distInCent == 0)
	{
		stop();
		return;
	}
	if (dia > 0)
	{
		setTarget((distInCent / (PI*dia) * 1440));
	}
	setTarget(distInCent);
}

inline void PIDMotor::setAngle(int angle, bool start)
{
	int countNow = totalCount % 1440;
	int requiredCount = angle * 4;
	int countDiff = abs(countNow - requiredCount) > abs(requiredCount - countNow) ? countNow - requiredCount : requiredCount - countNow;
	setTarget(countDiff);
	work = start;
}

inline void PIDMotor::rotate(int angle, bool start)
{
	setTarget(angle * 4);
	work = start;
}

inline void PIDMotor::rotateCW()
{
	targetSet = false;
	analogWrite(P1, 255);
	analogWrite(P2, 0);
}

inline void PIDMotor::rotateCCW()
{
	targetSet = false;
	analogWrite(P1, 0);
	analogWrite(P2, 255);
}

inline void PIDMotor::setTarget(long int countTarget)
{
	target = countTarget;
	totalCount += connectedEncoder->lastVal - accError;
	connectedEncoder->write(accError);
	updateNeeded = true;
	totalError = 0;
	targetSet = true;
}

inline void PIDMotor::addToTarget(long int countToAdd)
{
	target += countToAdd;
	if (!updateNeeded) setTarget(countToAdd);
	totalError = 0;
}

inline void PIDMotor::stop()
{
	analogWrite(P1, 0);
	analogWrite(P2, 0);
	updateNeeded = false;
	completeFraction = 1;
	target = connectedEncoder->lastVal;
	accError = connectedEncoder->lastVal - target;
}

inline void PIDMotor::update()
{
	if ((target > 0 && target <= connectedEncoder->lastVal) || (target < 0 && target >= connectedEncoder->lastVal)) stop();
	if (!(targetSet && work)) return;
	if (!updateNeeded) return;
	if (slave) slaveUpdate(); else masterUpdate();
	completeFraction = connectedEncoder->lastVal / target;
}

void PIDMotor::start()
{
	work = true;
}

inline void PIDMotor::makeMaster()
{
	slave = false;
	masterMotor = 0;
}

inline void PIDMotor::makeSlave(PIDMotor *master)
{
	slave = true;
	masterMotor = master;
}

inline void PIDMotor::masterUpdate()
{
	if (abs(connectedEncoder->lastVal) < AccDist)
	{
		if (target > 0)
		{
			speed = constrain(abs(connectedEncoder->lastVal) / AccDist * MaxSpeed, 100, MaxSpeed);
			analogWrite(P1, speed);
			analogWrite(P2, 0);
		}
		else
		{
			speed = constrain(abs(connectedEncoder->lastVal) / AccDist * MaxSpeed, 100, MaxSpeed);
			analogWrite(P2, speed);
			analogWrite(P1, 0);
		}
	}
	//Simple PID
	else if (abs(connectedEncoder->lastVal - target) < PIDDist)
	{
		time = millis();
		dt = lastTime - time;
		error = target > 0 ? target - connectedEncoder->lastVal : connectedEncoder->lastVal - target;
		totalError += error;
		de = error - lastError;
		speed = constrain(kp*error + kd*(de / dt) + ki*totalError*dt, 30, MaxSpeed);
		if (target > 0) { analogWrite(P1, speed); analogWrite(P2, 0); }
		else { analogWrite(P2, speed); analogWrite(P1, 0); }
		lastTime = time;
		lastError = error;
		if (speed == 30) updateNeeded = false;
	}
}

inline void PIDMotor::slaveUpdate()
{
	if (target == 0) return;
	subTarget = (long)(target * (float)masterMotor->completeFraction);
	time = millis();
	dt = lastTime - time;
	error = target > 0 ? subTarget - connectedEncoder->lastVal : connectedEncoder->lastVal - subTarget;
	totalError += abs(totalError)<100 ? error : 0;
	de = error - lastError;
	speed = kp*error + kd*((float)de / dt) + ki*totalError*dt;
	analogWrite(target > 0 ? P1 : P2, constrain(masterMotor->speed + speed, 0, 255));
	lastTime = time;
	lastError = error;

}

inline void PIDMotor::initClass()
{
	for (int i = 0 ; i < maxMotors ; i++) motorList[i] = 0;
	initialized = false;
}

inline void PIDMotor::updateAll()
{
	for (int i = 0; i < nMotors; i++) motorList[i]->update();
}

inline bool PIDMotor::isBusy()
{
	return (completeFraction >= 1);
}
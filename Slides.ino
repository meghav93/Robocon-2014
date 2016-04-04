#include "Declarations.h"

///The length of the horizontal slide in cm
#define maxX 90
///The length of the vertical slide in cm
#define maxZ 90
///The initial position of the horizontal slider
#define orgX 0
///The initial position of the vertical slider
#define orgZ maxZ

///Slides constructor
/**
 The Slides object needs 8 connections in total to control two motors.
 \param firstPin The pin number of the first pin connected in the order : motor Forward, Backward, Up, Down
 \param encPin1 The pin number of A pin of encoder 1. Order: Forward, Backward, Up, Down
 */
Slides::Slides(PinNum firstPin, PinNum encPin1)
{
	hEncoder = new encoder(encPin1);
	vEncoder = new encoder(encPin1 + 2);
	hMotor = new PIDMotor(firstPin, 60, hEncoder, 4);
	vMotor = new PIDMotor(firstPin + 2, 60, vEncoder, 4);
	slideSyncro->addMotor(hMotor);
	slideSyncro->addMotor(vMotor);
	reset();
}

///Reset motors and the encoders connected to the slides.
void Slides::reset()
{
	hMotor->reset();		hEncoder->reset();
	vMotor->reset();		vEncoder->reset();
	currentPosX = orgX;
	currentPosZ = orgZ;
	nextX = 0;
	nextZ = 0;
	movementSequence.clear();
}

///Move the slides by x and z distance in horizontal and vertical distance respectively
/**
 The motors are controlled so that the specified distance is reached. The distance is measured in cm from the current position of the slide. Previous move command, if it exists, is cancelled. If the total distance specified exceeds the maximum distance possible, the slide is moved to the maximum distance allowed.
 \param x Distance in horizontal direction, in cm
 \param z Distance in vertical direction, in cm
 \param lin If set to true, the slide moves to the point in a straight line, otherwise the sequence followed is either : Vertical then horizontal, if extending in horizontal direction or Horizontal then vertical, if retracting in horizontal direction
*/
void Slides::move(float x, float z, bool lin)
{
	nextX = 0;
	nextZ = 0;
	if (x + currentPosX > maxX) x = maxX - currentPosX;
	if (z + currentPosZ > maxZ) z = maxZ - currentPosZ;
	if (x + currentPosX < 0) x = -currentPosX;
	if (z + currentPosZ < 0) z = -currentPosZ;

	if (lin || x==0 || z==0) 
	{
		currentPosX += x;
		currentPosZ += z;
		hMotor->move(x,false);
		vMotor->move(z,false);
		slideSyncro->startAll();
	} else 
	{
		if (x > 0) nextX = x; else nextZ = z;
		if (nextX > 0)
		{
			vMotor->move(z);
			movementSequence.addIns(MOVE,x,0);
		} else {
			hMotor->move(x);
			movementSequence.addIns(MOVE,0,z);
		}
	}

}

///Move the slides in the specified direction by exactly one unit.
/**
 The directions accepted are Forward, Backward, Upward, Downward, ForwardUp, ForwardDown, BackwardUp, BackwardDown. All other values are ignored.
  \param moveDir The direction to move in
 */
void Slides::move(MoveDirection moveDir)
{
	movementSequence.clear();
	switch (moveDir)
	{
	case Forward:
		move(1,0,true);
		break;
	case Backward:
		move(-1,0,true);
		break;
	case Upward:
		move(0,1,true);
		break;
	case Downward:
		move(0,-1,true);
		break;
	case ForwardUp:
		move(1,1,true);
		break;
	case ForwardDown:
		move(1,-1,true);
		break;
	case BackwardUp:
		move(-1,1,true);
		break;
	case BackwardDown:
		move(-1,-1,true);
		break;
	default:
		return;
		break;
	}
}

///Starts the next instruction from its movement queue if not busy
void Slides::update()
{
	if (isBusy()) return;
	if (movementSequence.isEmpty()) return;
	MoveIns nextIns = movementSequence.getIns();
	if (nextIns.type == MOVE) move(nextIns.val1,nextIns.val2,true);
}

///Returns true if the slide is in middle of a moving operation
bool Slides::isBusy()
{
	return slideSyncro->isBusy();
}
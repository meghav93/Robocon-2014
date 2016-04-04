#include "Declarations.h"

///The message pulse delay in ms
#define MsgDelay 250

///PlateControl uses two motors and 4 electromagnets
/**
 \param motor1Pin1 The F pin of the swivel motor
 \param otherPin1 The pin number where the swivel encoder is connected, followed by the rotate encoder and the electromagnets
*/
PlateControl::PlateControl(PinNum motor1Pin1, PinNum otherPin1)
{
	swivelE = new encoder(otherPin1,360);
	rotateE = new encoder(otherPin1 + 2,360);
	swivelMotor = new PIDMotor(motor1Pin1, 60, swivelE, 10);
	rotateMotor = new PIDMotor(motor1Pin1 + 2, 60, rotateE, 10);
	em1 = new Electromagnet(otherPin1 + 4);
	em2 = new Electromagnet(otherPin1 + 5);
	em3 = new Electromagnet(otherPin1 + 6);
	em4 = new Electromagnet(otherPin1 + 7);
	synchro = new Synchronizer(0.3);
	synchro->addMotor(swivelMotor);
	synchro->addMotor(rotateMotor);
	reset();
}

///Reset all the devices connected to the mechanism
void PlateControl::reset()
{
	swivelMotor->reset();
	rotateMotor->reset();
	swivelE->reset();
	rotateE->reset();
}

///Actuate all the elctromagnets to pick up the child
void PlateControl::engage()
{
	em1->activate();
	em2->activate();
	em3->activate();
	em4->activate();
}

///Release the child
void PlateControl::disengage()
{
	em1->deactivate();
	em2->deactivate();
	em3->deactivate();
	em4->deactivate();
}

///Set the angle of the plate relative to 0,0(horizontal position)
/**
 \param y The swivel angle to be covered. negative value means swivel to the inside
 \param z The rotation angle. Negative means CCW direction
*/
void PlateControl::setAngle(int y, int z)
{
	swivelMotor->setAngle(y,false);
	rotateMotor->setAngle(z,false);
	synchro->startAll();
}

///Rotate the plate, relative to the current point
/**
 The movement made is always linear.
 \param y The incremental angle to be covered in swivel direction
 \param z The incremental angle to be covered in rotate directoin
*/
void PlateControl::rotate(int y,int z)
{
	swivelMotor->rotate(y,false);
	rotateMotor->rotate(z,false);
	synchro->startAll();
}
  
///Rotate the plate in any one direction by 1 degree
/**
 The directions allowed are : Upward, Downward, Left, Right, LeftUp, RightUp, LeftDown, RightDown. All other values are ignored
 \param rotateDir The direction to rotate the motor in
*/
void PlateControl::rotate(MoveDirection rotateDir)
{
	switch (rotateDir)
	{
	case Left:
		rotate(0,-1);
		break;
	case Upward:
		rotate(1,0);
		break;
	case Right:
		rotate(1,0);
		break;
	case Downward:
		rotate(-1,0);
		break;
	case UpLeft:
		rotate(1,-1);
		break;
	case UpRight:
		rotate(1,1);
		break;
	case DownLeft:
		rotate(-1,-1);
		break;
	case DownRight:
		rotate(-1,1);
		break;
	default:
		return;
		break;
	}
}

///Pass message to the child robot
/**
 Use the electromagnets to pass message to the child. Currently, four different messages are allowed: MsgSeesaw, MsgSwing, MsgPoleWalk, MsgJungleGyn
 \param msg The message to pass to the child.
*/
void PlateControl::passMessage(int msg)
{
	switch (msg)
	{
	case MsgSeesaw:
		em1->deactivate();
		delay(MsgDelay);
		em1->activate();
		break;
	case MsgSwing:
		em2->deactivate();
		delay(MsgDelay);
		em2->activate();
		break;
	case MsgPoleWalk:
		em3->deactivate();
		delay(MsgDelay);
		em3->activate();
		break;
	case MsgJungleGym:
		em4->deactivate();
		delay(MsgDelay);
		em4->activate();
		break;
	}
}
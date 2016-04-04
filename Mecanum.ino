#include "Declarations.h"

///The amount of distance the wheel should rotate when moving forward by 1 cm.
#define XFactor 1.414
///The amount of distance the wheel should rotate when moving sideways by 1 cm.
#define YFactor 1.414
///The distance equal to one unit, in cm.
#define OneUnit 0.5
///The turning radius of Mecanum in cm
#define TurningRadius 98

///Mecanum constructor initializes the associated variables and resets the mechanism
/**
 Eight pins are required in Mecanum for the motors and 8 for encoders.
 The diameter of the wheels is set as 15 cm, and the RPM of the motor is taken as 200. The PPR of each enncoder is considered to be 360
 The motor connections are made in order: Front: Left: F B Right: F B Back: Left: F B Right: F B
 Similarly, encoder connections are same order.
 \param MotorsPin1 The first of the PWM pins. The next 7 pins are considered to be in succession
 \param EncP1 The first pin of the first encoder. The rest of pins are considered to be in succession
*/
Mecanum::Mecanum(PinNum MotorsPin1,PinNum EncP1) 
{
	frontLeftE = new encoder(EncP1);
	frontRightE = new encoder(EncP1 + 2);
	backLeftE = new encoder(EncP1 + 4);
	backRightE = new encoder(EncP1 + 6);

	frontLeft = new PIDMotor(MotorsPin1, 200,frontLeftE, 15);
	frontRight = new PIDMotor(MotorsPin1 + 2, 200,frontRightE, 15);
	backLeft = new PIDMotor(MotorsPin1 + 4, 200, backLeftE,15);
	backRight = new PIDMotor(MotorsPin1 + 6, 200, backRightE,15);

	commonSynchronizer = new Synchronizer(0.2);
	commonSynchronizer->addMotor(frontLeft);
	commonSynchronizer->addMotor(frontRight);
	commonSynchronizer->addMotor(backLeft);
	commonSynchronizer->addMotor(backRight);
	reset();
}


///Reset all the mechanisms related to the Mecanum
/**
 Resets all the motors and the encoders connected to it
*/
void Mecanum::reset()
{
	frontLeft->reset();			frontLeftE->reset();
	frontRight->reset();		frontRightE->reset();
	backLeft->reset();			backLeftE->reset();
	backRight->reset();			backRightE->reset();
	currentAngle = 0;
	movementSequence.clear();
}

///Move the entire Mecanum relative to the current point
/**
 Moves the robot forward by x cm and sideways by y cm. This command is a background operation, ie, it sets the movement and it is possible to complete other activities while this is performed. If this method is called before the last activity is completed, the old target is replaced with new target from the current point. So, passing (0,0) will stop the robot where it is
 \param xInCm The distance to be moved in forward direction. Negative values make the robot move backwards.
 \param yInCm The distance to be moved in sideways direction. Positive is right, negative is left.
 \param localCoord If set to true, the robot moves in forward and sideways as seen from robot. If set  to false, the robot moves according to the coordinate system of the arena. Default is true. Global coordinate system may be buggy.
*/
void Mecanum::move(float xInCm,float yInCm,bool localCoord)
{
	if (!localCoord) 
	{
		float newX = xInCm*cos(currentAngle) - yInCm*sin(currentAngle);
		yInCm = xInCm*sin(currentAngle) + yInCm*cos(currentAngle);
		xInCm = newX;
	}
	frontLeft->move(xInCm * XFactor + yInCm * YFactor, false);
	frontRight->move(xInCm * XFactor - yInCm * YFactor, false);
	backLeft->move(xInCm * XFactor - yInCm * YFactor, false);
	backRight->move(xInCm * XFactor + yInCm * YFactor, false);
	commonSynchronizer->startAll();
}

///Move the entire Macanum by one unit in any one direction
/**
 The method moves the robot in any one of the base directions:
 Forward,Backward,Left,Right,ForwardLeft,BackwardLeft,ForwardRight,BackwardRight. Any other directions given are ignored.
 The robot stops after the movement. If previous target exists, it is discarded.
*/
void Mecanum::move(MoveDirection dir)
{
	movementSequence.clear();
	switch (dir)
	{
	case Forward:
		move(1,0);
		break;
	case Backward:
		move(-1,0);
		break;
	case Left:
		move(0,-1);
		break;
	case Right:
		move(0,1);
		break;
	case ForwardLeft:
		move(1,-1);
		break;
	case ForwardRight:
		move(1,1);
		break;
	case BackwardLeft:
		move(-1,-1);
		break;
	case BackwardRight:
		move(-1,1);
		break;
	default:
		return;
		break;
	}
}

///Rotate the Mecanum around the center of the square made by the wheels.
/**
 The procedure works by calculating the distance each wheel has to move. It is not possible to join the two methods, ie, move and rotate. All other motions are cancelled. If this motion is cancelled, the angle computed will be wrong.
 The rotation depends on the distance between the center of the square and any individual wheel, ie, the radius of turning.
 \param angle The angle the robot should rotate, in degrees, in clockwise direction.
*/
void Mecanum::rotate(int angle) 
{
	currentAngle += angle;
	float dist = angle * TurningRadius * PI/180.0;
	frontLeft->move(dist,false);
	frontRight->move(-dist,false);
	backLeft->move(dist,false);
	backRight->move(-dist,false);
	commonSynchronizer->startAll();
}

///Rotate the Mecanum by one unit
/**
 The method rotates the Mecanum around its center in clock wise or anti-clockwise direction as specified, by one unit, ie, one degree. All other motions are cancelled.
 \param rotDir The direction of rotation: CW for clockwise and CCW for Counter-clockwise
*/
void Mecanum::rotate(Direction rotDir)
{
	if (rotDir == CW) rotate(1); else rotate(-1);
}

///Clear the angle computed by the method
/**
 The method can be used to set the angle so as to remove mistakes in the angle computed. This method is used when the current angle of the robot is known.
 \param newAngleToSet The value of the current angle of the robot. Default value if 0.
*/
void Mecanum::clearAngle(int newAngleToSet)
{
	currentAngle = newAngleToSet;
}

///Stops all the motions of the robot.
void Mecanum::stop()
{
	commonSynchronizer->stopAll();
}

///Returns true if some motion is yet to complete
bool Mecanum::isBusy()
{
	return commonSynchronizer->isBusy();
}

///Starts the next instruction from its movement queue if not busy
void Mecanum::update()
{
	if (isBusy()) return;
	if (movementSequence.isEmpty()) return;
	MoveIns nextIns = movementSequence.getIns();
	if (nextIns.type == MOVE) move(nextIns.val1,nextIns.val2); else rotate((int)nextIns.val1);
}
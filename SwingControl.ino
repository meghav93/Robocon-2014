#include "Declarations.h"

///The length of the swing in cm
#define SwingLength 150
///The width of swing in cm
#define SwingWidth 30
///The angle to be covered in degree
#define SwingAngle 40
///Accuracy Resolution in degree
#define SwingAccuracy 1

///The initialization of the swing mechanism. One motor, two switches and one pneumatic is considered, and the mechanism uses the slides.
/**
 The swing control works by rotating two motors to 180 degrees. The swing is grabbed using pneumatic grippers and then the swing is pulled radially using the slides.
  \param motorPin1 The forward pin of the motors
  \param otherPins1 The first pin of the other components : two switches and two pins for a combined pneumatic gripper
  \param slides A pointer to the slides of the manual so that the robot can have control over it.
 */
SwingControl::SwingControl(PinNum motorPin1,PinNum otherPins1, Slides *slides)
{
	handleMotor = new PIDMotor(motorPin1,motorPin1 + 1,0);
	sensor1 = new Switch(otherPins1);
	sensor2 = new Switch(otherPins1 + 1);
	gripper = new Pneumatic(otherPins1 + 2);
	pSlides = slides;
	reset();
}

///The reset procedure of the Swing Control
/**
 The method only resets each of the components. The slides are not reset.
*/
void SwingControl::reset()
{
	handleMotor->reset();
	sensor1->reset();
	sensor2->reset();
	gripper->reset();
	gripper->open();
}

///Bring the handle into position using the motors.
/**
 The method is called before positioning the bot to hold the swing. Only the motors are brought in position. The swing is not grabbed. The command is negleted if the motors are already in position. No other action is allowed during this operation.
*/
void SwingControl::engage()
{
	if (sensor1->isPressed() && sensor2->isPressed()) return;
	handleMotor->rotateCW();
	while (!sensor1->isPressed() || !sensor2->isPressed());
	handleMotor->stop();
}

///Engage the gripper so as to hold the swing
/**
 The pneumatic is closed in this method. the method returns only after the pneumatic is closed(delay-based). If the engage() method has not been called, the call to this method is ignored.
*/
void SwingControl::hold()
{
	if (!sensor1->isPressed() || !sensor2->isPressed()) return;
	gripper->close(true);
}

///The action of moving the swing back in radial form is done here.
/**
 The entire motion for radial movement is entered in the slides instruction sequence. It is possible to perform other actions during the motion but it is highly not recommended.
 If the swing is not gripped, the method does not do anything.
*/
void SwingControl::operate()
{
	if (gripper->isOpen()) return;
	pSlides->movementSequence.clear();
	
	float centerX = 0, centerZ = SwingLength;
	float newX, oldX, newZ, oldZ;
	oldX = 0; oldZ = 0;

	for (int i = SwingAccuracy ; i <= SwingAngle; i += SwingAccuracy)
	{
		newX = centerX - SwingLength * sin(i * PI / 180);
		newZ = centerZ - SwingLength * cos(i * PI / 180);
		pSlides->movementSequence.addIns(MOVE,newX - oldX,newZ - oldZ);
		oldX = newX;
		oldZ = newZ;
	}
}

///Release the hold on the swing.
void SwingControl::release()
{
	gripper->open(true);
}

///Close the holder back. Delay-based
/**
 If the Pneumatic is still holding the swing, it is first released.
*/
void SwingControl::disengage()
{
	release();
	handleMotor->rotateCCW();
	delay(1000);
	handleMotor->stop();
}
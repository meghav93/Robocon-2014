#include "Declarations.h"

///The Manual constructor. Initializes all the related classes.
/**
 The constructor sets the pins for all the related classes. So, all the pins should be set here. All the values of the parts are reset. What the constructor does, is it calls the resetAll() function.
*/
Manual::Manual() 
{
	mecanum = new Mecanum(2,14);
	slides = new Slides(10,22);
	swingControl = new SwingControl(26,28,slides);
	seesawControl = new SeesawControl(31);
	plateControl = new PlateControl(33,37);
	resetAll();
}


///Reset all the mechanisms of the robot
/**
 The base idea is that all mechanisms have a way of resetting all their properties. The method simply resets all the mechanisms by calling their individual reset methods.
*/
void Manual::resetAll()
{
	mecanum->reset();
	slides->reset();
	swingControl->reset();
	plateControl->reset();
}


///Move the robot relative to current position by specified distance
/**
 The method is used for precise positioning of the robot. The method is not very practical for the arena as it depends on the current position of the robot. What this method can be used for, though, is quickly position the robot roughly near the required position. The rest can be done by manual positioning.
 \param xInCm The distance to be moved in forward direction in centimeters
 \param yInCm The distance to be moved in sideways direction in centimeters
 \param waitToComplete If set to true, the method waits for the Mecanum to complete the given translation before returning
*/
void Manual::move(float xInCm,float yInCm,bool waitToComplete)
{
	mecanum->move(xInCm,yInCm);
	if (!waitToComplete) return;
	while (mecanum->isBusy());
}

///Move the robot by one unit in the selected direction
/**
 The method moves the robot in any of the eight directions specified. The function is supposed to be used for manual control. After unit movement, the wheels stop. All the motors are continuosly synchronized with each step.
 The eight directions that the function accepts are : Left,Right,Forward,Backward, ForwardLeft, BackwardLeft, ForwardRight, BackwardRight
 Any other values are ignored.
 \param moveDir The direction to be travelled in.
*/
void Manual::move(MoveDirection moveDir)
{
	mecanum->move(moveDir);
}

///Stop the robot where it is
/**
 Stops all the motions and cancels all the tasks given to the robot.
*/
void Manual::stop()
{
	mecanum->stop();
}

///Rotate the robot at its location without change in position
/**
 Used for precise control rotation starting at the orientation of the robot. Should be used for setting the rough rotation angle of bot.
 \param angle Angle(in degrees) that the robot has to rotate
 \param waitToComplete If set to true, the method will wait for Mecanum to complete the rotation before returning
*/
void Manual::rotate(int angle,bool waitToComplete)
{
	mecanum->rotate(angle);
	if (!waitToComplete) return;
	while (mecanum->isBusy());
}

///Rotate the robot by one degree in the selected direction
/**
 The method rotates the bot about its center by one degree. All the motors are automatically synchronized so that the there is no translation of the robot. The motors are stopped after the degree rotation. Should be used for manual adjustment to the bot's rotation.
 \param dirOfRot CW for Clockwise rotation, CCW for counter-clockwise rotation
*/
void Manual::rotate(Direction dirOfRot) 
{
	mecanum->rotate(dirOfRot);
}

///Move the plate by the specified x and z coordinates from the current point.
/**
 The method precisely moves the plate to the specified x and z coordinates from the given point while keeping the motors in synchronization. The end position of the plate may not be exact as the end point depends on the start point. Manual control should be used to final position set.
 \param xInCm Horizontal distance in centimeters
 \param zInCm Vertical distance to be travelled, in centimeters
 \param linear If set true, a straight path to the point is calculated, else the robot moves in z direction first if extending, and x first if retracting.
 \param waitToComplete If set to true, the robot will wait till the motion has been completed.
*/
void Manual::movePlate(float xInCm,float zInCm,bool linear,bool waitToComplete)
{
	slides->move(xInCm,zInCm,linear);
	if (!waitToComplete) return;
	while (slides->isBusy());
}

///Move the plate by one unit in specified direction
/**
 The method is used for manual setting of the plate's location. The movement is always linear.The movement is stopped after movement of the motor(s) by one unit.
 The directions accepted are: Forward,Backward,Up,Down,ForwardUp,ForwardDown,BackwardUp,BackwardDown
 All other directions are ignored.
 \param moveDir The direction in which to move the plate.
*/
void Manual::movePlate(MoveDirection moveDir)
{
	slides->move(moveDir);
}

///Bring the swing handle in position
/**
Only the handles are brought in front of robot. The swing grippers are not closed and the position of the plate is not changed.
*/
void Manual::engageSwingControl()
{
	swingControl->engage();
}

///Reset the position of the swing handle
/**
The method releases the swing if it is held and retracts the swing handle. No changes to the plate position are made.
*/
void Manual::disengageSwingControl()
{
	swingControl->disengage();
}

///Engage the gripper to hold the swing
/**
If the swing handle is not engaged, the method will not do anything. The gripper should be closed only after positioning the gripper around the swing.
*/
void Manual::holdSwing()
{
	swingControl->hold();
}

///Pull back the swing
/**
The method should be called after positioning the gripper around the gripper. If the gripper is open, it is closed, and the swing will be pulled back so as to follow the radial direction. All the other operations of the robot are disabled for the sake of safety.
*/
void Manual::operateSwingControl()
{
	swingControl->operate();
}

///Bring the seesaw control in position
/**
 Operates the pneumatic cylinders so as to bring the seesaw operation mechanism in place. The mechanism is not automatically operated.
*/
void Manual::engageSeesawControl()
{
	seesawControl->engage();
}

///Reset the seesaw control back to original position
/**
 Operates the pneumatic cylinders so as to reset the seesaw operation mechanism.
*/
void Manual::disengageSeesawControl()
{
	seesawControl->disengage();
}

///Operate the seesaw for one lap
/**
 The seesaw is operated once. All other functions of the bot are disabled for safety. The method should be called after positioning the push-bar over the seesaw. If the mechanism is not in place, the method does not do anything.
 */
void Manual::operateSeesawControl()
{
	seesawControl->operate();
}

///Turn on the electromagnets to hold the child bot.
void Manual::engagePlate()
{
	plateControl->engage();
}

///Turn off the electromagnets to release the child bot.
void Manual::disengagePlate()
{
	plateControl->disengage();
}

///Set the plate angle to the precise angle
/**
 The angle here, is taken from the zero degrees as compared to the start position of the plate. So this angle is very accurate. But the final angle of the child bot depends on the orientation of the parent bot. Manual control should be used after setting the angle using this method.
 \param swivelAngle The angle of the Swivel angle of the plate
 \param zAngle The angle of rotation of the plate
 \param waitToComplete If set to true, the manual waits for the completion of the given task of rotation to the plate before returning.
 */
void Manual::setPlateAngle(int swivelAngle,int zAngle,bool waitToComplete)
{
	plateControl->setAngle(swivelAngle,zAngle);
}


///Rotate the plate in the given direction by one degree
/**
 The plate is rotated in the specified direction by one degree after which it is stopped. The method should be used for manual setting of the plate to precise location.
 The different directions possible are:
 Up, Down, Left, Right, UpLeft, UpRight, DownLeft, DownRight
 All other directions are ignored.
 \param rotateDir Direction in which to rotate the plate
*/
void Manual::rotatePlate(MoveDirection rotateDir)
{
	plateControl->rotate(rotateDir);
}

///Pass message to the Child Robot using Electromagnets.
/**
 If the plate is disengaged, the method returns without doing anything. 
 Four messages can be sent: Seesaw, Swing, PoleWalk and JungleGym
 \param message The message to be passed
*/
void Manual::passMessage(int message)
{
	plateControl->passMessage(message);
}
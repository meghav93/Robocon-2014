#ifndef DECL_H
#define DECL_H

#include <Arduino.h>
#include <Encoder.h>
#define PinNum char
#define Direction bool
#define CW true
#define CCW false
#define MsgSeesaw 1
#define MsgSwing 2
#define MsgPoleWalk 3
#define MsgJungleGym 4
#define MovementType bool
#define MOVE true
#define ROTATE false
#define MaxMoveIns 100
#define high(x) digitalWrite(x,HIGH)
#define low(x) digitalWrite(x,LOW)
#define state(x) digitalRead(x)
#define Open true
#define Close false
#define maxMotors 15
#define maxEncoders 15

enum MoveDirection {
	Forward, 
	Backward,
	Left, Upward,
	Right, Downward,
	ForwardLeft, ForwardUp, UpLeft,
	ForwardRight, ForwardDown, UpRight, 
	BackwardLeft, BackwardUp, DownLeft,
	BackwardRight, BackwardDown, DownRight
};

///A setup() function is defined by the header file itself
void setup();
///A loop() function is defined in the header file itself
void loop();

///For the main program, the Setup() function has to be defined
//void mainSetup();
///For the main program, the Loop() function has to be defined
//void mainLoop();

///A small structure to store the Movement instruction with its type.
struct MoveIns {
	MovementType type;
	float val1,val2;
};

///An instruction saving class to store all the instructions in sequence in queue form
class InsQueue {
	int totalIns;
	int curIns;
	void shiftQueue();	
	MoveIns queue[MaxMoveIns];
public:
	InsQueue();
	bool addIns(MovementType type,float arg1, float arg2);
	MoveIns getIns();
	void clear();
	bool isEmpty();
	bool isFull();
};

class Switch;
class Encoder;
class encoder;
class PIDMotor;
class Stepper;
class Pneumatic;
class Electromagnet;
class Synchronizer;
class Mecanum;
class Slides;
class SwingControl;
class SeesawControl;
class PlateControl;
class Manual;

///The main class that controls the movement of the manual bot. The Loop structure should only work with Manual object.
class Manual {
	Mecanum *mecanum;
	Slides *slides;
	SwingControl *swingControl;
	SeesawControl *seesawControl;
	PlateControl *plateControl;

	friend void loop();

public:
	void update();
	Manual();

	void resetAll();
	void move(float xInCm,float yInCm,bool waitToComplete = false);
	void move(MoveDirection moveDir);
	void stop();
	void rotate(int angle,bool waitToComplete = false);
	void rotate(Direction dirOfRot);
	void movePlate(float xInCm,float zInCm,bool linear=true,bool waitToComplete = false);
	void movePlate(MoveDirection moveDir);
	void engageSwingControl();
	void holdSwing();
	void operateSwingControl();
	void disengageSwingControl();
	void engageSeesawControl();
	void operateSeesawControl();
	void disengageSeesawControl();
	void engagePlate();
	void disengagePlate();
	void setPlateAngle(int swivelAngle,int zAngle,bool waitToComplete = false);
	void rotatePlate(MoveDirection rotateDir);
	void passMessage(int message);
};

///Mecanum class manages four motors with theiir motors and their encoders and keeps them in sync when necessary.
class Mecanum
{
	float currentAngle;
	PIDMotor *frontLeft,*frontRight,*backLeft,*backRight;
	Synchronizer *commonSynchronizer;
public:
	InsQueue movementSequence;
	encoder *frontLeftE,*frontRightE,*backLeftE,*backRightE;
	Mecanum(PinNum MotorsPin1,PinNum EncP1);
	void reset();
	void move(float xInCm,float yInCm,bool localCoord = true);
	void move(MoveDirection moveDir);
	void rotate(int angle);
	void rotate(Direction rotDir);
	void clearAngle(int newAngleToSet = 0);
	void stop();

	void update();

	bool isBusy();
};

///Slides controls the two slides and keeps them in sync, as specified.
class Slides
{
	PIDMotor *hMotor,*vMotor;
	encoder *hEncoder,*vEncoder;
	Synchronizer *slideSyncro;

	float currentPosX,currentPosZ;
	float nextX,nextZ;
public:
	InsQueue movementSequence;
	Slides(PinNum firstPin,PinNum encPin1);
	void reset();
	void move(float x,float z,bool lin);
	void move(MoveDirection moveDir);

	void update();

	bool isBusy();
};

///SwingControl controls the motors controlling the pulling mechanism of the swing and the pneumatics holding the swing.
///The class also controls the slides for the radial motion of the swing.
class SwingControl
{
	PIDMotor *handleMotor;
	Pneumatic *gripper;
	Switch *sensor1,*sensor2;
	Slides *pSlides;
public:
	SwingControl(PinNum motorPin1,PinNum otherPins1,Slides *slides);
	void reset();
	void engage();
	void hold();
	void operate();
	void release();
	void disengage();
};

///SeesawControl controls the four pneumatic cylinders that ready the seesaw operating mechanism
class SeesawControl
{
	Pneumatic *extension,*pushers;
public:
	SeesawControl(PinNum firstPin);
	void reset();
	void engage();
	void operate();
	void disengage();
};

///The plate control provides methods to set the rotation angle of the child bot and pass messages to it.
class PlateControl 
{
	PIDMotor *swivelMotor,*rotateMotor;
	Electromagnet *em1,*em2,*em3,*em4;
	encoder *swivelE,*rotateE;
	Synchronizer *synchro;
public:
	PlateControl(PinNum motor1Pin1,PinNum otherPin1);
	void reset();
	void engage();
	void disengage();
	void setAngle(int y,int z);
	void rotate(int y,int z);
	void rotate(MoveDirection rotateDir);
	void passMessage(int msg);
};

class encoder : public Encoder {
	static bool initialized;
	static encoder *encList[maxEncoders];
	static int numEnc;
public:
	encoder(PinNum p1,PinNum p2 = -1);
	static void initClass();
	static void updateAll();
};

class Stepper
{
	const PinNum P;
	float angle,targetAngle;
	const float stepAngle;
	const int waitTime;
	const bool HalfStep;
	int lastTime;

	PinNum highPin;
	void step(Direction rotDir);
public:
	Stepper(PinNum firstPin,float StepAngle,int delayBetweenPulses=1,bool halfStep = false);
	void setAngle(float newAngle);
	void update();
	void reset();
	void freewheel();
};

class Pneumatic 
{
	const PinNum P;
	bool state;
	const int ActTime;
public:
	Pneumatic(PinNum pin1,int actTime = 0);
	void reset();
	void close(bool waitToComplete = false){
	};
	void open(bool waitToComplete = false){
	};
	bool isBusy(){
		return false;
	};
	bool isOpen(){
		return false;
	};
};

class Electromagnet
{
public:
	Electromagnet(PinNum pin){
	};
	void reset(){
	};
	void activate(){
	};
	void deactivate(){
	};
};

class Switch
{
public:
	Switch(PinNum pin){
	};
	void reset(){
	};
	bool isPressed(){
		return false;
	};
};


class PIDMotor
{
	const PinNum P1, P2;
	long int target,subTarget;
	bool updateNeeded;
	float completeFraction;
	PIDMotor *masterMotor;

	long int lastTime, error, lastError, accError, totalError, time;
	bool slave;
	int dt, de, speed;
	float dia;
	
	static bool initialized;
	static int nMotors;
	static PIDMotor *motorList[maxMotors];

	void slaveUpdate();
	void masterUpdate();

	bool work,targetSet;

	long totalCount;
public:
	float kp, kd, ki;
	encoder *connectedEncoder;

	PIDMotor(PinNum P1, PinNum P2, encoder *e = 0,float diaInCm = -1);
	void reset();

	void move(float distInCent, bool start = true);
	void setAngle(int angle, bool start = true);
	void rotate(int angle, bool start = true);
	void rotateCW();
	void rotateCCW();
	void setTarget(long int countTarget);
	void update();
	void makeMaster();
	void makeSlave(PIDMotor *Master);
	void start();
	void stop();
	void addToTarget(long int countToAdd);
	bool isBusy();

	static void updateAll();
	static void initClass();
};
#endif


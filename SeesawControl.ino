#include "Declarations.h"

///The time to wait at the absolute bottom
#define SeesawDelay 1000

///SeesawControl consists of two Pneumatic connections, each consisting of two pneumatic cylinder connected as pair
/**
 \param firstPin The first pin of the two pneumatic cylinders. The first two belong to the engaging mechanism. The third and fourth pin are given to the seesaw operating mechanism.
*/
SeesawControl::SeesawControl(PinNum firstPin)
{
	extension = new Pneumatic(firstPin,500);
	pushers = new Pneumatic(firstPin + 2,500);
}

///Reset all the components of the object.
void SeesawControl::reset()
{
	extension->reset();
	pushers->reset();
	pushers->close();
}

///Bring the seesaw push-bar in position
/**
 This command does not operate the push-bar. If already open, the command is neglected. While the extension is performed, other activities are possible.
 */
void SeesawControl::engage()
{
	extension->open();
}

///Perform one lap of the push-bar. No other operations are possible during this time.
/**
 The operation of the Pneumatic cylinder is delay based. After the bar reaches the absolute bottom, it waits for a specific time, then returns to close. If the push bar is not in place, the method does nothing. All other operations are stopped.
*/
void SeesawControl::operate()
{
	if (!extension->isOpen()) return;
	pushers->close(true);
	delay(SeesawDelay);
	pushers->open(true);
}

///Return the push-bar to normal position
void SeesawControl::disengage()
{
	extension->close();
}
#include "Declarations.h"

InsQueue::InsQueue()
{
	totalIns = 0;
	curIns = 0;
}

bool InsQueue::addIns(MovementType type,float arg1, float arg2)
{
	queue[totalIns].type = type;
	queue[totalIns].val1 = arg1;
	queue[totalIns].val2 = arg2;
	totalIns++;
	if (totalIns > MaxMoveIns) if (!isFull()) shiftQueue(); else return false;
	return true;
}

MoveIns InsQueue::getIns()
{
	MoveIns res;
	if (curIns < totalIns) 
	{
		res = queue[curIns];
		curIns++;
	}
	return res;
}

void InsQueue::clear()
{
	totalIns = 0;
	curIns = 0;
}

bool InsQueue::isEmpty()
{
	return (curIns >= totalIns);
}

bool InsQueue::isFull()
{
	return (totalIns - curIns >= MaxMoveIns);
}

void InsQueue::shiftQueue()
{
	for (int i = curIns; i < totalIns; i++)
		queue[i - curIns] = queue[i];
	
	totalIns -= curIns;
	curIns = 0;
}
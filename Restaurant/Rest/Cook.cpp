#include "Cook.h"


Cook::Cook()
{
	status = AVAILABLE;
	AssignedOrder = nullptr;
	finishedDishesCount = 0;
	injuredThisTime =  false;
	injuredBefore = false;
	assignedInRest = false;
}


void Cook::setID (int id)
{
	ID = id;
}


int Cook::GetID () const
{
	return ID;
}


void Cook::setType(COOK_TYPE t)
{
	type = t;
}


COOK_TYPE Cook::GetType() const
{
	return type;
}


void Cook :: setSpeed ( int s )
{
	speed = s;
}


double Cook :: getSpeed ( ) const
{
	return speed;
}


void Cook :: setStatus ( COOK_STATUS st )
{
	status = st;
}


COOK_STATUS Cook :: getStatus () const
{
	return status;
}


/*void Cook :: setAssigOrderTime ( int t )
{
	AssigOrderTime = t;
}


int Cook :: getAssigOrderTime () const
{
	return AssigOrderTime;
}*/


void Cook :: setAssignedOrder ( Order* order )
{
	AssignedOrder = order;
}


Order* Cook :: getAssignedOrder () const
{
	return AssignedOrder;
}


void Cook :: setBreakDuration ( int bd )
{
	breakDuration = bd;
}


int Cook :: getBreakDuration () const
{
	return breakDuration;
}

void Cook :: setFinishedDishesCount ( int c )
{
	finishedDishesCount = c;
}


int Cook :: getFinishedDishesCount () const
{
	return finishedDishesCount;
}


/*void Cook :: setbeginBreakTime ( int t )
{
	beginBreakTime = t;
}


int Cook :: getbeginBreakTime () const
{
	return beginBreakTime;
}*/


/*bool Cook :: operator < ( Cook acook )
{
	return ( finishedDishesCount < acook.finishedDishesCount );
}


bool Cook :: operator <= ( Cook acook )
{
	return ( finishedDishesCount <= acook.finishedDishesCount );
}*/

void Cook :: setInjuredThisTime ( bool boolea  )
{
	injuredThisTime = boolea;
}

bool Cook :: getInjuredThisTime ()
{
	return injuredThisTime;
}

void Cook :: setInjuredBefore ( bool boolean )
{
	injuredBefore = boolean;
}
	
bool Cook :: getInjuredBefore ()
{
	return injuredBefore;
}

void Cook :: setAssignedInRest ( bool boolean )
{
	assignedInRest = boolean;
}

bool Cook :: getAssignedInRest ()
{
	return assignedInRest;
}

Cook::~Cook()
{
}

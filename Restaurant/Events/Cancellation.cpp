#include "Cancellation.h"
#include "..\Rest\Restaurant.h"


Cancellation::Cancellation(int etime, int ordID) :Event(etime, ordID)
{

}


void Cancellation::Execute(Restaurant* pRest)
{
	Order* pOrd = new Order(0, TYPE_NRM, 0, 0, 0);  //check this lw feh moshkela
	bool found = false;
	int i = 0;
	int c = 1;

	while (!found && i<c)
	{

		pRest->getEntryNormal(i, pOrd, c);
		if (pOrd->GetID() == OrderID)
		{
			pRest->removeNormal(i);
			found = true;
		}
		else 
			i++;
	}

}
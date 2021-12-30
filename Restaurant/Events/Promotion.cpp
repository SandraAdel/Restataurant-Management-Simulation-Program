#include "Promotion.h"
#include "..\Rest\Restaurant.h"
#include"Event.h"


void Promotion::Execute(Restaurant *pRest)
{
	Order* pOrd = new Order(0, TYPE_NRM, 0, 0, 0); 
	bool found = false;
	int i = 0;
	int c = 1;

	while (found==false && i<c)
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

	if ( found == true )
	{

	pOrd->setType(TYPE_VIP);
	pOrd->incrementTotalMoney(extraMoney);
	pRest->Add_OrderVip(pOrd);
	}

}
Promotion::Promotion()
{
}
Promotion::Promotion(int etime, int orderid, double exmon) :Event(etime, orderid)
{
	extraMoney = exmon;
}
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <string.h>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include"..\Generic_DS\UnsortedList.h"
#include"..\Generic_DS\SortedList.h"
#include"..\Generic_DS\PriorityData.h"

Restaurant::Restaurant() 
{
	pGUI = NULL;
	injuredCount = 0;
	urgentCount = 0;
	promotionCount = 0;
}
void Restaurant::Add_Order(Order* o )
{
	if (o->GetType()== TYPE_NRM )
	{
		int size = W_Order_Normal.getLength();
		W_Order_Normal.insert(size, o) ;
	}
	if (o->GetType()== TYPE_VGAN )
	{
		W_Order_Vegan.enqueue(o);

	}
	if(o->GetType()== TYPE_VIP)
	{
		float temp, money, size;
		int  arrivalTime;
		money = o->getTotalMoney();
		size = o->getSize();
		arrivalTime = o->getArrTime();
		temp = ( ( money - size ) / size ) - ( arrivalTime * 0.5 );
		int priority = ceil ( temp );

		PriorityData<Order*> pop (o , priority);
		W_Order_VIP.insertSortedDescendingly(pop);
	}
}
void Restaurant::Add_OrderVip(PriorityData<Order*> o)
{
	W_Order_VIP.insertSortedDescendingly(o);
}

void Restaurant::LoadingFunction(ifstream & input) // note that we passed the ifstream by & (why?)
{
	int N,G,V;
	int SN_Min,SN_Max,SG_Min,SG_Max,SV_Min,SV_Max;
	int SN,SG,SV;
	int BN_Min,BN_Max,BG_Min,BG_Max,BV_Min,BV_Max;
	int BN,BG,BV;
	int M;

	ORD_TYPE OrType;
	COOK_TYPE cooktype;

	if (input.is_open())
	{
		input>>N>>G>>V;
		input>>SN_Min>>SN_Max>>SG_Min>>SG_Max>>SV_Min>>SV_Max;
		input>>breakNumber>>BN_Min>>BN_Max>>BG_Min>>BG_Max>>BV_Min>>BV_Max;
		input>>injProp>>restPeriod;
		input>>autoP>>VIP_WT;
		input>>M;

		//int cookID = 1; 

		for(int ID=1;ID<=V;ID++)
		{srand(time(NULL));
		cooktype = CTYPE_VIP;
		Cook* VipCook=new Cook();

		VipCook->setType(cooktype);
		//setInjProp(injprop);
		//setRestPeriod(RstPrd);

		BV= rand() % ( BV_Max + 1 - BV_Min ) + BV_Min;
		if ( BV == 0 )
		{
			BV = BV_Min;
		}
		VipCook->setBreakDuration(BV);
		VipCook->setID(ID);

		SV= rand() % ( SV_Max +1 -SV_Min ) + SV_Min;
		if ( SV == 0 )
		{
			SV = SV_Min;
		}
		VipCook->setSpeed(SV);
		addAvailableCook(VipCook);

		//Av_cooks_VIP.enqueue(VipCook);             // ---> MOHAMED's PART
		//cookID++;
		}
		for(int ID=1;ID<=N;ID++)
		{srand(time(NULL));
		cooktype = CTYPE_NRM;
		Cook* NormalCook=new Cook();  // variables tet3emel fe class cook we ne3mel constructor keda

		NormalCook->setType(cooktype);

		//setInjProp(injprop);
		//setRestPeriod(RstPrd);

		BN= rand() % ( BN_Max +1 - BN_Min ) + BN_Min;
		if ( BN == 0 )
		{
			BN = BN_Min;
		}
		NormalCook->setBreakDuration(BN);
		NormalCook->setID(ID+V);

		SN= rand() % ( SN_Max +1 - SN_Min ) + SN_Min;
		if ( SN == 0 )
		{
			SN = SN_Min;
		}
		NormalCook->setSpeed(SN);
		addAvailableCook(NormalCook);
		//AV_Cooks_Normal.enqueue(NormalCook);       // ---> MOHAMED's PART
		//cookID++;
		}
		for(int ID=1;ID<=G;ID++)
		{srand(time(NULL));
		cooktype = CTYPE_VGAN;
		Cook* VeganCook=new Cook();

		VeganCook->setType(cooktype);

		//setInjProp(injprop);
		//setRestPeriod(RstPrd);

		BG= rand() % ( BG_Max +1 - BG_Min ) + BG_Min;
		if ( BG == 0 )
		{
			BG = BG_Min;
		}
		VeganCook->setBreakDuration(BG);
		VeganCook->setID(ID+V+N);

		SG= rand() % ( SG_Max +1 -SG_Min ) + SG_Min;
		if ( SG == 0 )
		{
			SG = SG_Min;
		}
		VeganCook->setSpeed(SG);
		addAvailableCook(VeganCook);
		//AV_Cooks_vegan.enqueue(VeganCook);         // ---> MOHAMED's PART 
		//cookID++;
		}

		/////////////////////////////////////////



		for(int l=1; l<=M; l++)
		{
			char temp,orderType;
			int one ,two , three,four;
			input >> temp ;

			if(temp == 'R')
			{
				input>> orderType >> one >> two >> three >> four;

				if(orderType=='N')
					OrType=TYPE_NRM;
				if(orderType=='G')
					OrType=TYPE_VGAN;
				if(orderType=='V')
					OrType=TYPE_VIP;


				Event* event= new ArrivalEvent((ORD_TYPE)OrType,one,two,three,four); // variables tet7at fel class we ne3melhom fe constructor
				EventsQueue.enqueue(event);
			}
			if(temp== 'X')
			{
				input>>one >> two ;

				Event* event= new Cancellation(one,two);
				EventsQueue.enqueue(event);
			}
			if(temp=='P')
			{
				input>> one>> two >> three;
				Event* event= new Promotion (one,two,three);
				EventsQueue.enqueue(event);
			}
		}
		pGUI->PrintMessage("loaded.......click to continue");
		pGUI->waitForClick();
		input.close();
	}
	else
	{pGUI->PrintMessage ("Unable to open file.......click to continue");

	pGUI->waitForClick();

	}
}

void Restaurant :: OutputFunction(ofstream & output)
{	
	if (output.is_open())
	{
		int NoOfNormalOrders=0, NoOfVeganOrders=0, NoOfVIPOrders=0;
		int TotalWait = 0, TotalServe = 0, totalOrders = 0;
		Order* order;
		output<<"FT    "<<"ID     "<<"AT     "<<"WT     "<<"ST     "<<endl;
		finished.toArray(totalOrders);
		for(int i=0;i<totalOrders;i++)
		{
			finished.peekFront(order);
			output << order->getFinishTime()<<"    "<<order->GetID()<<"      "<<order->getArrTime()<<"      "<<order->getWaitTime()<<"      "<<order->getServTime()<<endl;
			order->GetType();
			if(order->GetType()==TYPE_NRM)
				NoOfNormalOrders++;
			else if(order->GetType()==TYPE_VGAN)
				NoOfVeganOrders++;
			else if(order->GetType()==TYPE_VIP)
				NoOfVIPOrders++;
			TotalWait = TotalWait + order->getWaitTime();
			TotalServe = TotalServe + order ->getServTime();
			finished.dequeue(order);
		}
		output<<"Orders: ";
		output<<totalOrders;
		output<<" [Norms:";
		output<<NoOfNormalOrders;
		output<<", Veg:";
		output<<NoOfVeganOrders;
		output<<", VIP:";
		output<<NoOfVIPOrders;
		output<<"]"<<endl;


		output<<"Cooks: ";
		output<<(AV_Cooks_Normal.getLength())+(AV_Cooks_vegan.getLength())+(Av_cooks_VIP.getLength());
		output<<" [Norms:";
		output<<(AV_Cooks_Normal.getLength());
		output<<", Veg:";
		output<<(AV_Cooks_vegan.getLength());
		output<<", VIP:";
		output<<(Av_cooks_VIP.getLength());
		output<<", Injured:";
		output<<getInjuredCount();
		output<<"]"<<endl;

		float avgWait = TotalWait/totalOrders;
		float avgServe = TotalServe/totalOrders;
		output<<"Avg Wait = ";
		output<<avgWait;
		output<<", Avg Serve = ";
		output<<avgServe<<endl;


		output<<"Urgent Orders: ";
		output<<getUrgentCount();        
		output<<",  Auto Promoted: ";
		float no = (getPromotionCount()/NoOfNormalOrders);
		output<< no * 100;
		output<<"%"<<endl;

		output.close();
	}
}


void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();
	int mod ;
	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		mod=1;
			Simulation(mod);
			break;
	case MODE_STEP:
		mod=2;
			Simulation(mod);
			break;
	case MODE_SLNT:
		mod=0;
		Simulation(mod);
		break;



	};

}


//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event *pE;
	while( EventsQueue.peekFront(pE) )	//as long as there are more events
	{
		if(pE->getEventTime() != CurrentTimeStep )	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}


Restaurant::~Restaurant()
{
	if (pGUI)
		delete pGUI;
}
bool Restaurant::getEntryNormal( int position, Order* & p, int & c )
{

	W_Order_Normal.getEntry(position, p);
	c = W_Order_Normal.getLength();
	return true;
}

bool Restaurant::removeNormal(int i)
{
	W_Order_Normal.remove(i);
	return true;
}

void Restaurant :: incrementOrdersWaitTime ()
{
	if ( !W_Order_VIP.isEmpty() )
	{
		PriorityData<Order*> vipOrder;
		for ( int i=0; i<W_Order_VIP.getLength(); i++ )
		{
			W_Order_VIP.getEntry(i, vipOrder);
			vipOrder.getData() ->incrementWaitTime();
		}
	}

	if ( !W_Order_Normal.isEmpty() )
	{
		Order* normalOrder;
		for ( int i=0; i<W_Order_Normal.getLength(); i++ )
		{
			W_Order_Normal.getEntry(i, normalOrder);
			normalOrder ->incrementWaitTime();
		}
	}
}

void Restaurant :: addAvailableCook(Cook* avCook)
{
	int cookProductivity = avCook ->getSpeed() - avCook ->getBreakDuration();
	PriorityData<Cook*> cookToAdd ( avCook, cookProductivity );

	if ( avCook ->GetType() == CTYPE_VIP )
	{
		Av_cooks_VIP.insertSortedDescendingly(cookToAdd);
	}
	else if ( avCook ->GetType() == CTYPE_NRM )
	{
		AV_Cooks_Normal.insertSortedDescendingly(cookToAdd);
	}
	else if ( avCook ->GetType() == CTYPE_VGAN )
	{
		AV_Cooks_vegan.insertSortedDescendingly(cookToAdd);
	}
}

int Restaurant :: getBreakNumber () const
{
	return breakNumber;
}

int Restaurant :: getRestPeriod () const
{
	return restPeriod;
}

int Restaurant :: getInjuredCount () const
{
	return injuredCount;
}

float Restaurant :: getInjProp () const
{
	return injProp;
}
int Restaurant::getAutoP() const
{
	return autoP;
}
int Restaurant::getVIP_WT() const
{
	return VIP_WT;
}
int Restaurant::getUrgentCount() const
{
	return urgentCount;
}
int Restaurant::getPromotionCount() const
{
	return promotionCount;
}
void Restaurant :: setInjProp (float injprop)
{
	injProp = injprop;
}
void Restaurant :: setRestPeriod (int rstperiod)
{
	restPeriod=rstperiod;
}
bool Restaurant :: assignCook( Cook* cook, Order* order, int currentTimestep )
{
	if ( order ->getSize() > getBreakNumber() - cook ->getFinishedDishesCount() )
	{
		return false;
	}
	else if ( order ->getSize() <= getBreakNumber() - cook ->getFinishedDishesCount() )
	{
		//Removal of cook from the list of availables

		if ( cook ->getStatus() == AVAILABLE )   // Only remove from the list of availables if the cook assigned was previously available, not on break or in rest
		{
			if ( cook ->GetType() == CTYPE_VIP )
			{
				Av_cooks_VIP.remove(cook);
			}
			else if ( cook ->GetType() == CTYPE_NRM )
			{
				AV_Cooks_Normal.remove(cook);
			}
			else if ( cook ->GetType() == CTYPE_VGAN )
			{
				AV_Cooks_vegan.remove(cook);
			}

		}
		//Necessary changes in corresponding cook and order

		cook ->setStatus(BUSY);
		cook ->setAssignedOrder(order);
		cook ->setFinishedDishesCount( cook ->getFinishedDishesCount() + order ->getSize() );
		servicedOrders(order,currentTimestep); 
		//Adding cook to list of busy cooks

		float temp = ( order ->getSize() / cook ->getSpeed() ) + currentTimestep;
		int orderFinishtime = ceil ( temp );
		PriorityData<Cook*> busyCook ( cook, orderFinishtime );
		Busy_Cooks.insertSortedAscendingly(busyCook);

		return true;
	}
	else 
	{return false;}
}

void Restaurant :: checkBusyCooks ( int currentTimestep )
{
	if ( !Busy_Cooks.isEmpty() )
	{

	PriorityData<Cook*> cookPriorityData;
	bool newCook;
	newCook = Busy_Cooks.getEntry( 0, cookPriorityData );  // false if the list is empty, so we won't check

	while ( cookPriorityData.getPriority() == currentTimestep && newCook )
	{
		//General changes when cook finishes order

		Cook* cook = cookPriorityData.getData();
		Busy_Cooks.remove(cook);
		finishedOrders( cook ->getAssignedOrder(), currentTimestep );
		cook ->setAssignedOrder(nullptr);

		if ( cook ->getFinishedDishesCount() == getBreakNumber() )   // when it is time for the cook's break whether he/she is injured or not
		{
			//General changes when cook takes a break

			cook ->setFinishedDishesCount(0);
			cook ->setStatus(ON_BREAK);

			//Further changes if he/she was injured

			if ( cook ->getInjuredThisTime() == true )
			{
				//cook ->setSpeed ( cook ->getSpeed() * 2 );  // equivalent to doubling his priority while cooking, which was done in injureCook function.
				cook ->setInjuredThisTime (false);
			}

			//Adding cook to list of on break cooks

			int breakFinishTime = currentTimestep + cook ->getBreakDuration();
			PriorityData<Cook*> onBreakCook ( cook, breakFinishTime );
			On_Break_Cooks.insertSortedAscendingly( onBreakCook );
		}
		else if ( cook ->getFinishedDishesCount() < getBreakNumber() && cook ->getInjuredThisTime() == false )   // if the cook is not injured and it is not his break time
		{
			// Add cook to list of availables

			cook ->setStatus(AVAILABLE);
			addAvailableCook(cook);
		}
		else if ( cook ->getFinishedDishesCount() < getBreakNumber() && cook ->getInjuredThisTime() == true )   // if it is not the cook's break time but he is injured
		{
			// changes after injured finishes his order

			cook ->setInjuredThisTime (false);
			//cook ->setSpeed ( cook ->getSpeed() * 2 );   // equivalent to doubling his priority while cooking, which was done in injureCook function.
			cook ->setStatus (IN_REST);

			// Adding cook to list of in rest cooks

			int restFinishTime = currentTimestep + restPeriod;
			PriorityData<Cook*> InRestCook ( cook, restFinishTime );
			In_Rest_Cooks.insertSortedAscendingly( InRestCook );
		}

		newCook = Busy_Cooks.getEntry( 0, cookPriorityData );  // false if the list is empty, so we won't check further
	}

	}
}

void Restaurant :: finishedOrders ( Order* order, int currentTimestep )
{
	// change order data members accordingly

	order ->setServiceTime ( currentTimestep - (order ->getArrTime()) - order ->getWaitTime() );
	order ->updateFinishTime();
	order ->setStatus (DONE);
	if(order->GetType()== TYPE_NRM)
	{
		served_norm_order++;
	}
	else if(order->GetType()== TYPE_VGAN)
	{
		served_vegan_order++;
	}
	else if(order->GetType()== TYPE_VIP)
	{
		served_vip_order++;
	}
	// finding position of order to remove it from list of in-service orders

	Order* dummyOrder;
	bool found = false;
	int i = 0;

	while ( i < In_service.getLength() && !found )
	{
		In_service.getEntry ( i, dummyOrder );

		if ( dummyOrder == order )
		{
			In_service.remove(i);
			found = true;
		}

		i++;
	}

	// Add order to queue of finished orders

	finished.enqueue(order);
}

void Restaurant :: injureCook ()
{
	if ( !Busy_Cooks.isEmpty() )
	{

	//Generating random probability

	float n = rand() % 100;
	float R = n / 100;

	if ( R <= injProp )
	{
		//Making first busy cook injured ( MUST BE CALLED AFTER REMOVING COOKS WHO FINISHED THEIR ORDERS (i.e after checkBusyCooks) )

		PriorityData<Cook*> injuredCook;
		Busy_Cooks.getEntry( 0, injuredCook );

		int i = 1;
		bool cond = true;
		while ( cond && injuredCook.getData() ->getInjuredThisTime() == true )    // if the first the cook in the list is already injured, look for the next healthy one until, if any 
		{
			cond = Busy_Cooks.getEntry( i, injuredCook );
			i++;
		}

		if ( cond == true )
		{

		injuredCook.getData() ->setInjuredThisTime(true);  // injuring cook
		if ( injuredCook.getData() ->getInjuredBefore() == false ) //counting only if it was the first time for the cook to get injured, as a cook can get injured more than once
		{
			injuredCount++;
			injuredCook.getData() ->setInjuredBefore(true);
		}

		//removing cook, making necessary changes due to injury then re-positioning him in the list

		Busy_Cooks.remove( injuredCook.getData() );
		injuredCook.setPriority ( injuredCook.getPriority() * 2 );  // EQUIVALENT to having his/her speed halved --> which is the next line
		//injuredCook.getData() ->setSpeed( injuredCook.getData()->getSpeed() * 0.5 );  
		Busy_Cooks.insertSortedAscendingly( injuredCook );

		}

	}

	}
}

void Restaurant :: checkCooksInBreakAndRest ( int currentTimestep )
{
	if ( !On_Break_Cooks.isEmpty() )
	{

	// Checking on break cooks

	PriorityData<Cook*> cookPriorityData1;
	bool newCook1;
	newCook1 = On_Break_Cooks.getEntry ( 0, cookPriorityData1 );

	while ( newCook1 && cookPriorityData1.getPriority() == currentTimestep )  // his break has finished
	{
		// Remove him from list of on break cooks

		Cook* cook = cookPriorityData1.getData();
		On_Break_Cooks.remove(cook);

		// General changes for this type of cook

		cook ->setStatus ( AVAILABLE );

		// Further changes if this cook was assigned in rest before

		if ( cook ->getAssignedInRest() == true )
		{
			cook ->setSpeed ( cook ->getSpeed() * 2 );
			cook ->setAssignedInRest ( false );
		}

		// Adding cook to list of avaialables

		addAvailableCook ( cook );

		newCook1 = On_Break_Cooks.getEntry ( 0, cookPriorityData1 );
	}

	}

	//-----------------------------------------------------

	if ( !In_Rest_Cooks.isEmpty() )
	{

	// checking on cooks in rest

	PriorityData<Cook*> cookPriorityData2;
	bool newCook2;
	newCook2 = In_Rest_Cooks.getEntry ( 0, cookPriorityData2 );

	while ( newCook2 && cookPriorityData2.getPriority() == currentTimestep )  // his resing period is over
	{
		// Removing cook from list of cooks taking rest

		Cook* cook = cookPriorityData2.getData();
		In_Rest_Cooks.remove(cook);

		// Necessary changes in this type of cook & adding him to list of availables

		cook ->setStatus ( AVAILABLE );
		addAvailableCook ( cook );

		newCook2 = In_Rest_Cooks.getEntry ( 0, cookPriorityData2 );
	}

	}
}

bool Restaurant :: cooksOnBreakAlternative ( Order* urgentOrder, int currentTimestep )  //MUST BE CALLED AFTER REMOVING COOKS WHO FINISHED THEIR BREAK ( i.e. after checkcooksInBreakAndRest )
{                                                                                       //so, the cook assigned has not finished his break yet
	if ( !On_Break_Cooks.isEmpty() )                                                    //if he was assigned in rest before, his speed will not go back to normal yet because he has not had a full break (or rest before) yet
	{
	
	PriorityData<Cook*> cookPriorityData;                                               
	bool cookFound = false;

	// Searching for a VIP cook in the list

	int i1 = 0;

	while ( !cookFound && i1 < Busy_Cooks.getLength() )
	{
		Busy_Cooks.getEntry ( i1, cookPriorityData );

		if ( cookPriorityData.getData() ->GetType() == CTYPE_VIP )
		{
			cookFound = true;
		}

		i1++;
	}

	// Searching for a normal one if a VIP one is not found

	int i2 = 0;

	while ( !cookFound && i2 < Busy_Cooks.getLength() )
	{
		Busy_Cooks.getEntry ( i2, cookPriorityData );

		if ( cookPriorityData.getData() ->GetType() == CTYPE_NRM )
		{
			cookFound = true;
		}

		i2++;
	}

	// Searching for a vegan one if neither VIP nor normal was found

	int i3 = 0;

	while ( !cookFound && i3 < Busy_Cooks.getLength() )
	{
		Busy_Cooks.getEntry ( i3, cookPriorityData );

		if ( cookPriorityData.getData() ->GetType() == CTYPE_VGAN )
		{
			cookFound = true;
		}

		i3++;
	}

	if ( cookFound == true )  //if a suitable cook was found
	{
		Busy_Cooks.remove( cookPriorityData.getData() );                            //Removal 

		bool cond = assignCook ( cookPriorityData.getData(), urgentOrder, currentTimestep );    //Assignment

		return cond;      //if a cook from this list was assigned or not
	}
	    return false;    // Double Checking if the list was empty

	}

	return false; //the list was empty so no cook was assigned
	             
}

bool Restaurant :: cooksInRestAlternative ( Order* urgentOrder, int currentTimestep )
{
	if ( !In_Rest_Cooks.isEmpty() )
	{

	PriorityData<Cook*> cookPriorityData;                                              
	bool cookFound = false;

	// Searching for a VIP cook in the list

	int i1 = 0;

	while ( !cookFound && i1 < In_Rest_Cooks.getLength() )
	{
		In_Rest_Cooks.getEntry ( i1, cookPriorityData );

		if ( cookPriorityData.getData() ->GetType() == CTYPE_VIP )
		{
			cookFound = true;
		}

		i1++;
	}

	// Searching for a normal one if a VIP one is not found

	int i2 = 0;

	while ( !cookFound && i2 < In_Rest_Cooks.getLength() )
	{
		In_Rest_Cooks.getEntry ( i2, cookPriorityData );

		if ( cookPriorityData.getData() ->GetType() == CTYPE_NRM )
		{
			cookFound = true;
		}

		i2++;
	}

	// Searching for a vegan one if neither VIP nor normal was found

	int i3 = 0;

	while ( !cookFound && i3 < In_Rest_Cooks.getLength() )
	{
		In_Rest_Cooks.getEntry ( i3, cookPriorityData );

		if ( cookPriorityData.getData() ->GetType() == CTYPE_VGAN )
		{
			cookFound = true;
		}

		i3++;
	}

	if ( cookFound == true )  //if a suitable cook was found
	{
		In_Rest_Cooks.remove( cookPriorityData.getData() );                                          //Removal

		cookPriorityData.getData() ->setAssignedInRest (true);                                       //changing data of injured cook was did not take suitable rest (did not complete it)
		cookPriorityData.getData() ->setSpeed ( cookPriorityData.getData() ->getSpeed() * 0.5 );

		bool cond = assignCook ( cookPriorityData.getData(), urgentOrder, currentTimestep );                     // Assignment

		return cond;       //if cook from this list was assigned or not
	}
	
	   return false;   //Double Checking if the list was empty

	}

	 return false;       //the list was empty so no cook was assigned
}

void Restaurant::checkUrgentOrders()
{
	if (W_Order_VIP.isEmpty())
		return;


	PriorityData<Order*> urgent;
	int i = 0;
	int j = 0;
	int VIPordersCount = W_Order_VIP.getLength();
	while (i < VIPordersCount )
	{
		W_Order_VIP.getEntry(i-j, urgent);
		if (urgent.getData()->getWaitTime() >= VIP_WT)
		{
			W_Order_VIP.remove(urgent.getData());
			W_Order_Urgent.enqueue(urgent.getData());
			urgentCount++;
			j++;
		}
		i++;
	}
}
void Restaurant::checkAutoPromotion()
{
	if (W_Order_Normal.isEmpty())
		return;

	Order* order;
	int i = 0;
	int j = 0;
	int NordersCount = W_Order_Normal.getLength();
	while (i < NordersCount )
	{
		W_Order_Normal.getEntry(i-j, order);
		if (order->getWaitTime() >= autoP)  
		{
			W_Order_Normal.remove(i-j);
			order->setType(TYPE_VIP);
			Add_Order(order);
			promotionCount++;
			j++;
		}
		i++;
	}
}
void Restaurant::serviceVipOrder(int timeStep)
{
	while (W_Order_VIP.isEmpty())
		return;

	int i = 0;
	int c = 0;
	int l = W_Order_VIP.getLength();
	while (i < l)
	{

		bool found = false;

		PriorityData<Order*> pOrd;
		PriorityData<Cook*> pCook;
		W_Order_VIP.getEntry(c, pOrd);

		//assigning it to a VIP cook
		int v = 0;
		while (v < Av_cooks_VIP.getLength() && found == false)
		{
			Av_cooks_VIP.getEntry(v, pCook);
			found = assignCook(pCook.getData(), pOrd.getData(), timeStep);
			v++;
		}


		//assigning it to a normal cook if VIP cook not found
		int n = 0;
		while (n < AV_Cooks_Normal.getLength() && found == false)
		{
			AV_Cooks_Normal.getEntry(n, pCook);
			found = assignCook(pCook.getData(), pOrd.getData(), timeStep);
			n++;
		}


		//assigning it to a vegan cook if neither VIP nor normal cook is found
		int g = 0;
		while (g < AV_Cooks_vegan.getLength() && found == false)
		{
			AV_Cooks_vegan.getEntry(g, pCook);
			found = assignCook(pCook.getData(), pOrd.getData(), timeStep);
			g++;
		}

		//if it wasn't assigned to a cook go to next order
		if (found == false)
		{
			c++;
		}

		i++;
	}
}


void Restaurant::serviceVeganOrder(int timeStep)
{
	while (W_Order_Vegan.isEmpty())
		return;

	int i = 0;
	int c = 0;

	W_Order_Vegan.toArray(c);

	while (i < c)
	{

		bool found = false;
		Order* pOrd;
		PriorityData<Cook*> pCook;
		W_Order_Vegan.peekFront(pOrd);

		int g = 0;
		while (g < AV_Cooks_vegan.getLength() && found == false)
		{
			AV_Cooks_vegan.getEntry(g, pCook);
			found = assignCook(pCook.getData(), pOrd, timeStep);
			g++;
		}

		i++;
	}

}
void Restaurant::serviceNormalOrder(int timeStep)
{
	while (W_Order_Normal.isEmpty())
		return;

	int i = 0;
	int c = 0;
	int l = W_Order_Normal.getLength();
	while (i < l)
	{


		bool found = false;

		Order* pOrd;
		PriorityData<Cook*> pCook;
		W_Order_Normal.getEntry(c, pOrd);

		//assigning it to a normal cook

		int n = 0;
		while (n < AV_Cooks_Normal.getLength() && found == false)
		{
			AV_Cooks_Normal.getEntry(n, pCook);
			found = assignCook(pCook.getData(), pOrd, timeStep);
			n++;
		}


		//assigning it to a VIP cook if normal cook not found
		int v = 0;
		while (v < Av_cooks_VIP.getLength() && found == false)
		{
			Av_cooks_VIP.getEntry(v, pCook);
			found = assignCook(pCook.getData(), pOrd, timeStep);
			v++;
		}

		//if this order wasn't assigned for whatever reason, go to the next order
		if (found == false)
		{
			c++;
		}
		i++;

	}
}

void Restaurant::serviceUrgentOrder(int timeStep)
{
	while (W_Order_Urgent.isEmpty())
		return;

	int i = 0;
	int c = 0;
	W_Order_Urgent.toArray(c);

	while (i < c)
	{
		bool found = false;

		Order* pOrd;
		PriorityData<Cook*> pCook;
		W_Order_Urgent.peekFront(pOrd);

		//assigning it to a VIP cook
		int v = 0;
		while (v < Av_cooks_VIP.getLength() && found == false)
		{
			Av_cooks_VIP.getEntry(v, pCook);
			found = assignCook(pCook.getData(), pOrd, timeStep);
			v++;
		}


		//assigning it to a normal cook if VIP cook not found
		int n = 0;
		while (n < AV_Cooks_Normal.getLength() && found == false)
		{
			AV_Cooks_Normal.getEntry(n, pCook);
			found = assignCook(pCook.getData(), pOrd, timeStep);
			n++;
		}


		//assigning it to a vegan cook if neither VIP nor normal cook is found
		int g = 0;
		while (g < AV_Cooks_vegan.getLength() && found == false)
		{
			AV_Cooks_vegan.getEntry(g, pCook);
			found = assignCook(pCook.getData(), pOrd, timeStep);
			g++;
		}

		//if there are no available cooks we see if there are any on break cooks
		if (found == false)
		{
			found = cooksOnBreakAlternative(pOrd, timeStep);
		}

		//if found is still false meaning that there are no available cooks or on break then we take from rest if possible
		if (found == false)
		{
			found = cooksInRestAlternative(pOrd, timeStep);
		}

		i++;
	}

}

void Restaurant::servicedOrders(Order* order, int timeStep)
{
	if (order->GetType() == TYPE_VIP)
	{

		bool found1 = W_Order_VIP.remove(order);

		if (found1 == false) //if the order wasn't removed from waiting VIP orders then this means that this order is an urgent order (in the waiting urgent orders list)
		{
			W_Order_Urgent.dequeue(order);
		}

	}


	else if (order->GetType() == TYPE_NRM)
	{
		bool found2 = false;
		int i = 0;
		Order* pOrd;

		while (i < W_Order_Normal.getLength() && found2 == false)
		{
			W_Order_Normal.getEntry(i, pOrd);

			if (order->GetID() == pOrd->GetID())
			{
				W_Order_Normal.remove(i);
				found2 = true;
			}
			else
				i++;

		}
	}

	else if (order->GetType() == TYPE_VGAN)
	{
		W_Order_Vegan.dequeue(order);
	}


	order->setStatus(SRV);
	order->setWaitingTime(timeStep - order->getArrTime());
	In_service.insert(In_service.getLength(), order);

}

void Restaurant::FillDrawingList()
{
	//It should add ALL orders and Cooks to the drawing list
	//It should get orders from orders lists/queues/stacks/whatever (same for Cooks)
	//To add orders it should call function  void GUI::AddToDrawingList(Order* pOrd);
	//To add Cooks it should call function  void GUI::AddToDrawingList(Cook* pCc);

	Order*pOrd;
	Cook*pCc;
	int size = 999;
	Order** WVIP_Orders_Array = W_Order_VIP.toArray(size); 
	for(int i=0; i<size; i++)
	{
		pOrd=WVIP_Orders_Array[i];
		pGUI->AddToDrawingList(pOrd);

	}
	Order** WN_Orders_Array = W_Order_Normal.toArray(size);
	for(int i=0; i<size; i++)
	{
		pOrd = WN_Orders_Array[i];
		pGUI->AddToDrawingList(pOrd);


	}

	Order** WV_Orders_Array = W_Order_Vegan.toArray(size);
	for(int i=0; i<size; i++)
	{
		pOrd = WV_Orders_Array[i];
		pGUI->AddToDrawingList(pOrd);

	}
	Order** WU_Orders_Array = W_Order_Urgent.toArray(size);
	for(int i=0; i<size; i++)
	{
		pOrd = WU_Orders_Array[i];
		pGUI->AddToDrawingList(pOrd);

	}
	Order**pIn_service= In_service.toArray(size);
	for(int i=0; i<size; i++)
	{
		pOrd = pIn_service[i];
		pGUI->AddToDrawingList(pOrd);

	}
	Order**pfinished= finished.toArray(size);
	for(int i=0; i<size; i++)
	{
		pOrd = pfinished[i];
		pGUI->AddToDrawingList(pOrd);

	}



	Cook**Vip_Cook=Av_cooks_VIP.toArray(size);
	for(int i=0; i<size; i++)
	{
		pCc=Vip_Cook[i];
		pGUI->AddToDrawingList(pCc);

	}

	Cook**N_Cook=AV_Cooks_Normal.toArray(size);
	for(int i=0; i<size; i++)
	{
		pCc=N_Cook[i];
		pGUI->AddToDrawingList(pCc);



	}

	Cook**Ve_Cook=AV_Cooks_vegan.toArray(size);
	for(int i=0; i<size; i++)
	{
		pCc=Ve_Cook[i];
		pGUI->AddToDrawingList(pCc);


	}



}



void Restaurant::Simple_simulator()
{
	ifstream fofo;
	string filename;	
	//Order* pOrd;
	//Event* pEv;


	pGUI->PrintMessage("Simple simulator.  >=  I/P filename:");

	fofo.open(pGUI->GetString());
	LoadingFunction(fofo);

	pGUI->PrintMessage("Events should be loaded from a file...CLICK to continue");
	pGUI->waitForClick();

	int CurrentTimeStep = 1;

	pGUI->UpdateInterface();
	//as long as events queue is not empty yet
	while(!EventsQueue.isEmpty() || !In_service.isEmpty() )
	{
		//print current timestep
		char timestep[100];
		itoa(CurrentTimeStep,timestep,100);	
		pGUI->PrintMessage(timestep);
		Order*poord;
		ExecuteEvents(CurrentTimeStep);
		//execute all events at current time step
		FillDrawingList();
		pGUI->UpdateInterface();
		pGUI->waitForClick();

		if(!W_Order_VIP.isEmpty())
		{
			W_Order_VIP.remove(poord);
			poord->setStatus(SRV);
			int size = In_service.getLength();
			In_service.insert(size, poord);

			pGUI->AddToDrawingList(poord);
		}

		if (!W_Order_Normal.isEmpty())
		{
			W_Order_Normal.getEntry(0,poord);
			W_Order_Normal.remove(0);
			poord->setStatus(SRV);
			int size = In_service.getLength();
			In_service.insert(size, poord);

			pGUI->AddToDrawingList(poord);

		}
		if(!W_Order_Vegan.isEmpty())
		{
			W_Order_Vegan.dequeue(poord);
			poord->setStatus(SRV);
			int size = In_service.getLength();
			In_service.insert(size, poord);

			pGUI->AddToDrawingList(poord);
		}

		if (CurrentTimeStep % 5==0)
		{   
			if ( !In_service.isEmpty() )
			{
				for ( int i=0; i<3; i++ )
				{  if (!In_service.isEmpty() )
				{Order*pooord;
				In_service.getEntry(0,pooord);
				In_service.remove(0);
				pooord->setStatus(DONE);
				finished.enqueue(pooord);
				pGUI->AddToDrawingList(pooord);
				}
				}
			}
		}

		pGUI->UpdateInterface();
		CurrentTimeStep++;
		pGUI->ResetDrawingList();
		int count;
		if(!finished.isEmpty() )
		{Order** poooord= finished.toArray(count);
		for(int i=0;i<count;i++)

		{
			pGUI->AddToDrawingList(poooord[i]);
		}
		}
		if(!In_service.isEmpty() )
		{Order** poooord=In_service.toArray(count);
		for(int i=0;i<count;i++)

		{
			pGUI->AddToDrawingList(poooord[i]);
		}
		}
		pGUI->waitForClick();
	}

	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();
}





void Restaurant::Simulation(int mod)
{
	///===============data members of input loading functions=========================///
	ifstream inputO;
	string user_input;
	string sms;
	///==============================================================================///
	served_norm_order=0,served_vip_order=0,served_vegan_order=0;
	///===========input loading step=====///
	pGUI->PrintMessage("welcome , to cufe Resturant simulation >= please enter the input filename");
	user_input= pGUI->GetString();
	inputO.open(user_input);
	LoadingFunction(inputO);
	pGUI->PrintMessage("Events should be loaded from a file...CLICK to continue");
	pGUI->waitForClick();
	///============================================================================///
	ofstream Restaurant_sim ;
	///=========time stepper (counter)===========///
	int CurrentTimeStep=1;  ///----> current step data meber with initial step 1

	while(!EventsQueue.isEmpty() || !In_service.isEmpty() || !W_Order_Normal.isEmpty() || !W_Order_Vegan.isEmpty() || !W_Order_Urgent.isEmpty() || !W_Order_VIP.isEmpty())
	{
		int waiting_N_O ;
		W_Order_Normal.toArray(waiting_N_O);
		int waiting_G_O;
		W_Order_Vegan.toArray(waiting_G_O);
		int waiting_V_O;
		W_Order_VIP.toArray(waiting_V_O);
		int av_cn , av_cg ,av_cv;
		AV_Cooks_Normal.toArray(av_cn);
		AV_Cooks_vegan.toArray(av_cg);
		Av_cooks_VIP.toArray(av_cv);

		//============print current timestep=================//
		char timestep[100];
		itoa(CurrentTimeStep,timestep,10);	
		sms=timestep;
		string message = "Ts = "+sms + " / "+"W NORM O :"+to_string(waiting_N_O) +" / "+"W vegan O :"+to_string(waiting_G_O)+" / "+"W vip O :"+to_string(waiting_V_O)+ " / "+"AV vegan COOKS : "+to_string(av_cg)+ " / "+ "AV NORM COOKS : "+to_string(av_cn)+ " / "+"AV VIP COOKS : "+to_string(av_cv)+ "/ >= / "+"served_norm : "+to_string(served_norm_order)+ " / "+"served_vegan : "+to_string(served_vegan_order)+ " / "+"served_VIP : "+to_string(served_vip_order);
		//==================================================================================//
		ExecuteEvents(CurrentTimeStep);// =======>> execute all events at current time step
		
		
		if(mod==2)
		{pGUI->PrintMessage(message);
			FillDrawingList();
		pGUI->UpdateInterface();
			Sleep(500);}
		else if(mod==1)
		{pGUI->PrintMessage(message);
			FillDrawingList();
			pGUI->UpdateInterface();
			pGUI->waitForClick();
		}
		//==========================check the cooks avability============================//

		checkCooksInBreakAndRest(CurrentTimeStep); //====>> check break and rests of the cooks

		checkBusyCooks(CurrentTimeStep);//========>> check busy cooks 

		//==============================check the orders===================================//
		incrementOrdersWaitTime ();

		checkUrgentOrders(); // ==========>> urgent the urgent orders need to promotion  

		checkAutoPromotion(); // =========>> check auto promotion orders 

		/// ===========================assign orders===================================//
		serviceUrgentOrder(CurrentTimeStep);

		serviceVipOrder(CurrentTimeStep);

		serviceNormalOrder(CurrentTimeStep);

		serviceVeganOrder(CurrentTimeStep);

		//==============================check the injury============================//
		injureCook();
		//============================================================================//

		CurrentTimeStep++;
		if(mod==2)
		{pGUI->ResetDrawingList();
			Sleep(500);}
		else if(mod==1)
		{pGUI->ResetDrawingList();
		}
	
	}
	string output = "OUT_"+ user_input;
		pGUI->ResetDrawingList();
		pGUI->UpdateInterface();
	pGUI->PrintMessage("end of the simulation......click to exit");
	Restaurant_sim.open(output);
	OutputFunction(Restaurant_sim);
	pGUI->waitForClick();
	Sleep(1000);
}


#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

// Graphics includes
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"

#include<dos.h>
#include<conio.h>
//data structude includes
#include "..\Generic_DS\Queue.h"
#include "..\Generic_DS\SortedList.h"
#include "..\Generic_DS\UnsortedList.h"
#include"..\Generic_DS\PriorityData.h"
//class includes
#include "Order.h"
#include "..\Defs.h"
//events includes
#include"..\Events\ArrivalEvent.h"
#include"..\Events\Cancellation.h"
#include"..\Events\Promotion.h"
#include "..\Events\Event.h"
//loading functions includes
#include<fstream>


// it is the maestro of the project

class Restaurant  
{	
private:
	///=================================================Data memebers=============================////
	GUI *pGUI;
	int count;
	int breakNumber;           // --> the no of dishes a cook has to completes before he takes a break ( const for all cooks of ALL types )
	int restPeriod;
	int injuredCount;
	float injProp;
	int autoP; //autopromotion input
	int VIP_WT; //waiting limit input
	int urgentCount; //count of urgent orders
	int promotionCount; //count of autopromotions
	 int served_norm_order,served_vip_order,served_vegan_order; // served orders counter


	///=======================================================================================///

	                      //====data structure of events====//

	Queue<Event*> EventsQueue;	      //Queue of all events that will be loaded from file


	                       //=======data structure of orders========//

	Queue<Order*> W_Order_Urgent;   //Queue of urgent orders
	SortedList<Order*> W_Order_VIP;//sorted list of waiting vip orders
	UnsortedList<Order*>W_Order_Normal;//sorted list of waiting normal orders
	Queue<Order*> W_Order_Vegan ;     //queue of waiting vegan orders
	UnsortedList<Order*> In_service;//nsorted list of orders that in service 
	Queue<Order*> finished ;// queue of finished orders



	                            //======data structure of cooks=====//

	SortedList<Cook*> Av_cooks_VIP;//sorted list of avalible vip cooks
	SortedList<Cook*> AV_Cooks_Normal;//sorted list cooks of available normal cooks
	SortedList<Cook*> AV_Cooks_vegan;//sorted of avalible vegan cooks
	SortedList<Cook*>Busy_Cooks; // sorted list of busy cooks 
	SortedList<Cook*>On_Break_Cooks;// sorted list of cooks who are on break
	SortedList<Cook*>In_Rest_Cooks; // sorted list of injured cooks who are in rest

///========================================================================================================///
public:
	

	Restaurant();
	~Restaurant();
	///===================================order functions==================================///
	void Add_Order(Order*);
	void Add_OrderVip(PriorityData<Order*>o);
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void incrementOrdersWaitTime ();
	///==============================================================================================///
	                            ///============ setters & getters=============/// 
	bool getEntryNormal(int position, Order* & p, int & c);
	bool removeNormal(int position);
	void addAvailableCook(Cook* avCook);

	int getBreakNumber () const;     // --> to check if a cook needs a break
	int getRestPeriod () const;      // --> to put injured cook in list of resting cooks
	int getInjuredCount () const;
	float getInjProp () const;
	int getAutoP() const;
	int getVIP_WT() const;
	int getUrgentCount() const;
	int getPromotionCount() const;
	void setInjProp (float injprop);
    void setRestPeriod (int rstperiod);
	///=========================================================================================//
	                  //==================Functions of Assignment==================//

	bool assignCook( Cook* cook, Order* order, int currentTimestep );
	void checkBusyCooks ( int currentTimestep );
	void finishedOrders ( Order* order, int currentTimestep );
	void injureCook ();
	void checkCooksInBreakAndRest ( int currentTimestep );
	bool cooksOnBreakAlternative ( Order* urgentOrder, int currentTimestep );
	bool cooksInRestAlternative ( Order* urgentOrder, int currentTimestep );

	void checkUrgentOrders();  //checks if there are VIP orders that waited longer than the limit
	void checkAutoPromotion();  //autopromotes normal orders that waited longer than autoP
	void serviceVipOrder(int timeStep);  //assigns VIP orders to cooks if possible
	void serviceVeganOrder(int timeStep);  //assigns vegan orders to cooks if possible
	void serviceNormalOrder(int timeStep);  //assigns normal orders to cooks if posssible
	void serviceUrgentOrder(int timeStep);  //assigns urgent orders to cooks if possible
	void servicedOrders(Order* order, int timeStep);  //adds the assigned order to In_service list

	void FillDrawingList();

	///===========================================================================================================///

	
	                                 /// ===============input & output functions=============/// 
	void LoadingFunction(ifstream & input); // note that we passed the ifstream by & (why?)
	void OutputFunction(ofstream & output);
	/// ==================================================================================================///
	                        ///==============Simulation=================///
	void RunSimulation();
	void Simple_simulator();
	void Simulation(int mod);
	/// ================================================================================================== 
	/*/// ===================    DEMO-related functions. Should be removed in phases 1&2   ================= 

	void Just_A_Demo();	//just to show a demo and should be removed in phase1 1 & 2
	void AddtoDemoQueue(Order* po);	//adds an order to the demo queue*/
	
};

#endif
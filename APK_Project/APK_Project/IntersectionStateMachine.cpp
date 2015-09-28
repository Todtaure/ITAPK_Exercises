#pragma once
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <iostream>
#include <thread>

namespace sc = boost::statechart;

/*
State translations:
LightState_1	NS-Green, EW-Red
LightState_2	NS-Yellow, EW-Red
InitialRR		NS-Red, EW-Red
LightState_4	NS-RedYellow, EW-Red
LightState_4	NS-Red, EW-Green
LightState_5	NS-Red, EW-Yellow
LightState_6	NS-Red, EW-RedYellow

*/

enum LightStates
{
	NORMALEXECUTION,
	INITIALRR,
	LS_1,
	LS_2,
	LS_3,
	LS_4,
	LS_5,
	LS_6
};

int const wait_ = 2;

/*
Events
*/
struct EvToLS1 : sc::event<EvToLS1> {};
struct EvToLS2 : sc::event<EvToLS2> {};
struct EvToLS3 : sc::event<EvToLS3> {};
struct EvToLS4 : sc::event<EvToLS4> {};
struct EvToLS5 : sc::event<EvToLS5> {};
struct EvToLS6 : sc::event<EvToLS6> {};
struct EvToRR : sc::event<EvToRR> {};

struct EvEVApproaching : sc::event<EvEVApproaching> {};
struct EvEVPassed : sc::event<EvEVPassed> {};


/*
StateMachine
*/

struct Operational;
struct YellowFlashing;

struct Machine : sc::state_machine<Machine, Operational>
{
	//Entry
	Machine()
	{

	}

	//Exit
	~Machine()
	{

	}

	bool emergency_interrupt_ = false;
	bool error_interupt_ = false;
};


/*
Tier-I states
*/

struct NormalExecution;
struct Emergency;

struct Operational : sc::simple_state<Operational, Machine, NormalExecution>
{
	//Entry
	Operational()
	{

	}

	//Exit
	~Operational()
	{

	}
};

struct YellowFlashing : sc::simple_state<YellowFlashing, Machine>
{
	//Entry
	YellowFlashing()
	{
		std::cout << "Error state entered. Intersection " << std::endl;
	}

	//Exit
	~YellowFlashing()
	{

	}
};


/*
Tier-II states
*/
struct InitialRR;
struct InitialRR_Emergency;

struct NormalExecution : sc::simple_state<NormalExecution, Operational, InitialRR>
{
	//Entry
	NormalExecution()
	{
		previous_state_ = LightStates::NORMALEXECUTION;
	}

	//Exit
	~NormalExecution()
	{

	}

	LightStates previous_state_;

	//Events
	typedef sc::transition<EvEVApproaching, Emergency> reactions;
};

struct Emergency : sc::simple_state<Emergency, Operational>
{
	//Entry
	Emergency()
	{
		std::cout << "***" << std::endl << "Entering emergency state!" << std::endl << "***" << std::endl;
	}

	//Exit
	~Emergency()
	{

	}
};

/*
Tier-III states
*/
struct LightState_1;
struct LightState_2;
struct LightState_3;
struct LightState_4;
struct LightState_5;
struct LightState_6;

/*
* NorthSouth: Red
* EastWest: Red
*/
struct InitialRR : sc::simple_state<InitialRR, NormalExecution>
{
	//Entry
	InitialRR()
	{
		std::cout << "***" << std::endl;
		std::cout << "Initial state." << std::endl << "North-South: Red" << std::endl << "East-West: Red" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(wait_));
	}

	//Exit
	~InitialRR()
	{
		context<NormalExecution>().previous_state_ = INITIALRR;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvToLS6, LightState_6>, sc::transition<EvToLS3, LightState_3> > reactions;
};

/*
* NorthSouth: Green
* EastWest: Red
*/
struct LightState_1 : sc::simple_state<LightState_1, NormalExecution>
{
	//Entry
	LightState_1()
	{
		std::cout << "***" << std::endl << "LightState _1." << std::endl << "North-South: Green" << std::endl << "East-West: Red" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(wait_));
	}

	//Exit
	~LightState_1()
	{
		context<NormalExecution>().previous_state_ = LS_1;
	}

	//Events
	typedef sc::transition<EvToLS2, LightState_2> reactions;
};

/*
* NorthSouth: Yellow
* EastWest: Red
*/
struct LightState_2 : sc::simple_state<LightState_2, NormalExecution>
{
	//Entry
	LightState_2()
	{
		std::cout << "***" << std::endl << "LightState_2" << std::endl << "North-South: Yellow" << std::endl << "East-West: Red" << std::endl;
		
		std::this_thread::sleep_for(std::chrono::seconds(wait_));
	}

	//Exit
	~LightState_2()
	{
		context<NormalExecution>().previous_state_ = LS_2;
	}

	//Events
	typedef sc::transition<EvToRR, InitialRR> reactions;
};

/*
* NorthSouth: RedYellow
* EastWest: Red
*/
struct LightState_3 : sc::simple_state<LightState_3, NormalExecution>
{
	//Entry
	LightState_3()
	{
		std::cout << "***" << std::endl << "LightState_3" << std::endl << "North-South: RedYellow" << std::endl << "East-West: Red" << std::endl;
		
		std::this_thread::sleep_for(std::chrono::seconds(wait_));
	}

	//Exit
	~LightState_3()
	{
		context<NormalExecution>().previous_state_ = LS_3;
	}

	//Events
	typedef sc::transition<EvToLS1, LightState_1> reactions;
};

/*
* NorthSouth: Red
* EastWest: Green
*/
struct LightState_4 : sc::simple_state<LightState_4, NormalExecution>
{
	//Entry
	LightState_4()
	{
		std::cout << "***" << std::endl << "LightState_4" << std::endl << "North-South: Red" << std::endl << "East-West: Green" << std::endl;
		
		std::this_thread::sleep_for(std::chrono::seconds(wait_));
	}

	//Exit
	~LightState_4()
	{
		context<NormalExecution>().previous_state_ = LS_4;
	}

	//Events
	typedef sc::transition<EvToLS5, LightState_5> reactions;
};

/*
* NorthSouth: Red
* EastWest: Yellow
*/
struct LightState_5 : sc::simple_state<LightState_5, NormalExecution>
{
	//Entry
	LightState_5()
	{
		std::cout << "***" << std::endl << "LightState_5" << std::endl << "North-South: Red" << std::endl << "East-West: Yellow" << std::endl;
		
		std::this_thread::sleep_for(std::chrono::seconds(wait_));
	}

	//Exit
	~LightState_5()
	{
		context<NormalExecution>().previous_state_ = LS_5;
	}

	//Events
	typedef sc::transition<EvToRR, InitialRR> reactions;
};

/*
* NorthSouth: Red
* EastWest: RedYellow
*/
struct LightState_6 : sc::simple_state<LightState_6, NormalExecution>
{
	//Entry
	LightState_6()
	{
		std::cout << "***" << std::endl << "LightState_6" << std::endl << "North-South: Red" << std::endl << "East-West: RedYellow" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(wait_)); 
	}

	//Exit
	~LightState_6()
	{
		context<NormalExecution>().previous_state_ = LS_6;
	}

	//Events
	typedef sc::transition<EvToLS4, LightState_4> reactions;
};


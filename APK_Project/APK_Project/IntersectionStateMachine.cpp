#pragma once
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <iostream>
#include <thread>
#include "SCUtils.h"
#include <vector>

namespace sc = boost::statechart;
using namespace SCUtils;


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
#define TIMER std::chrono::seconds(1)
#define EM_TIMER std::chrono::seconds(4)

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

struct EvEmS1 : sc::event<EvEmS1> {};

struct EvEmS2 : sc::event<EvEmS2>
{
	EvEmS2(Direction ev_direction) : navigate_to_(ev_direction)
	{}

	Direction navigate_to_;
};

struct EvEmS3 : sc::event<EvEmS3>
{
	EvEmS3(Direction ev_direction) : navigate_to_(ev_direction)
	{}

	Direction navigate_to_;
};

struct EvEmS4 : sc::event<EvEmS4> {};

struct EvEmRR : sc::event<EvEmRR>
{
	EvEmRR(Direction ev_direction) : navigate_to_(ev_direction)
	{}

	Direction navigate_to_;
};

struct EvEVApproaching : sc::event<EvEVApproaching> {};
struct EvEVPassed : sc::event<EvEVPassed> {};


/*
StateMachine
*/

struct Operational;
struct YellowFlashing;
struct Emergency;

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

	void set_emergency_interrupt(Direction direction)
	{
		emergency_interrupt_ = true;
		ev_direction_ = direction;
	}

	bool emergency_interrupt_ = false;
	bool error_interupt_ = false;

	Direction ev_direction_;
};


/*
Tier-I states
*/

struct NormalExecution;
struct Emergency;

struct Operational : sc::simple_state<Operational, Machine, NormalExecution>
{
	//Entry
	Operational() : previous_state_(LightStates::NORMALEXECUTION)
	{

	}

	//Exit
	~Operational()
	{

	}

	LightStates previous_state_;
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

struct EM_LightState_1;
struct EM_LightState_2;
struct EM_LightState_3;
struct EM_LightState_4;
struct EM_InitialRR;
struct EM_Choice;

struct NormalExecution : sc::simple_state<NormalExecution, Operational, InitialRR>
{
	//Entry
	NormalExecution()
	{
	}

	//Exit
	~NormalExecution()
	{

	}

	//Events
};

struct Emergency : sc::state<Emergency, Operational, EM_Choice>
{
	//Entry
	Emergency(my_context ctx) : my_base(ctx)
	{
		std::cout << "***" << std::endl << "Entering emergency state!" << std::endl << "***" << std::endl;		
	}

	//Exit
	~Emergency()
	{
		std::cout << "***" << std::endl << "Leaving emergency state!" << std::endl << "***" << std::endl;
		context<Machine>().emergency_interrupt_ = false;
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
struct InitialRR : sc::state<InitialRR, NormalExecution>
{
	//Entry
	InitialRR(my_context ctx) : my_base(ctx)
	{
		//Check for emergency vehicle approaching.
		if (context<Machine>().emergency_interrupt_) { post_event(EvEVApproaching()); return; }

		std::cout << "***" << std::endl;
		std::cout << "Initial state." << std::endl << "North-South: Red" << std::endl << "East-West: Red" << std::endl;

		std::this_thread::sleep_for(TIMER);

		//Nort-South have had green, thus its East-West's turn
		if (context<Operational>().previous_state_ == LS_2 || context<Operational>().previous_state_ == NORMALEXECUTION)
		{
			std::cout << " *EVENT* " << "PostEvent ToSL6" << std::endl;
			post_event(EvToLS6());
		}
		else
		{
			std::cout << " *EVENT* " << "PostEvent ToSL3" << std::endl;
			post_event(EvToLS3());
		}
	}

	//Exit
	~InitialRR()
	{
		context<Operational>().previous_state_ = INITIALRR;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvToLS6, LightState_6>, sc::transition<EvToLS3, LightState_3>, sc::transition<EvEVApproaching, Emergency> > reactions;
};

/*
* NorthSouth: Green
* EastWest: Red
*/
struct LightState_1 : sc::state<LightState_1, NormalExecution>
{
	//Entry
	LightState_1(my_context ctx) : my_base(ctx)
	{
		//Check for emergency vehicle approaching.
		if (context<Machine>().emergency_interrupt_) { post_event(EvEVApproaching()); return; }

		std::cout << "***" << std::endl << "LightState _1." << std::endl << "North-South: Green" << std::endl << "East-West: Red" << std::endl;

		std::this_thread::sleep_for(TIMER);

		std::cout << " *EVENT* " << "PostEvent ToSL2" << std::endl;
		post_event(EvToLS2());
	}

	//Exit
	~LightState_1()
	{
		context<Operational>().previous_state_ = LS_1;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvToLS2, LightState_2>, sc::transition<EvEVApproaching, Emergency> > reactions;
};

/*
* NorthSouth: Yellow
* EastWest: Red
*/
struct LightState_2 : sc::state<LightState_2, NormalExecution>
{
	//Entry
	LightState_2(my_context ctx) : my_base(ctx)
	{
		//Check for emergency vehicle approaching.
		if (context<Machine>().emergency_interrupt_) { post_event(EvEVApproaching()); return; }

		std::cout << "***" << std::endl << "LightState_2" << std::endl << "North-South: Yellow" << std::endl << "East-West: Red" << std::endl;

		std::this_thread::sleep_for(TIMER);

		std::cout << " *EVENT* " << "PostEvent EvToRR" << std::endl;
		post_event(EvToRR());
	}

	//Exit
	~LightState_2()
	{
		context<Operational>().previous_state_ = LS_2;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvToRR, InitialRR>, sc::transition<EvEVApproaching, Emergency> > reactions;
};

/*
* NorthSouth: RedYellow
* EastWest: Red
*/
struct LightState_3 : sc::state<LightState_3, NormalExecution>
{
	//Entry
	LightState_3(my_context ctx) : my_base(ctx)
	{
		//Check for emergency vehicle approaching.
		if (context<Machine>().emergency_interrupt_) { post_event(EvEVApproaching()); return; }

		std::cout << "***" << std::endl << "LightState_3" << std::endl << "North-South: RedYellow" << std::endl << "East-West: Red" << std::endl;

		std::this_thread::sleep_for(TIMER);

		std::cout << " *EVENT* " << "PostEvent ToSL1" << std::endl;
		post_event(EvToLS1());
	}

	//Exit
	~LightState_3()
	{
		context<Operational>().previous_state_ = LS_3;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvToLS1, LightState_1>, sc::transition<EvEVApproaching, Emergency> > reactions;
};

/*
* NorthSouth: Red
* EastWest: Green
*/
struct LightState_4 : sc::state<LightState_4, NormalExecution>
{
	//Entry
	LightState_4(my_context ctx) : my_base(ctx)
	{
		//Check for emergency vehicle approaching.
		if (context<Machine>().emergency_interrupt_) { post_event(EvEVApproaching()); return; }

		std::cout << "***" << std::endl << "LightState_4" << std::endl << "North-South: Red" << std::endl << "East-West: Green" << std::endl;

		std::this_thread::sleep_for(TIMER);

		std::cout << " *EVENT* " << "PostEvent ToSL5" << std::endl;
		post_event(EvToLS5());
	}

	//Exit
	~LightState_4()
	{
		context<Operational>().previous_state_ = LS_4;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvToLS5, LightState_5>, sc::transition<EvEVApproaching, Emergency> >  reactions;
};

/*
* NorthSouth: Red
* EastWest: Yellow
*/
struct LightState_5 : sc::state<LightState_5, NormalExecution>
{
	//Entry
	LightState_5(my_context ctx) : my_base(ctx)
	{
		//Check for emergency vehicle approaching.
		if (context<Machine>().emergency_interrupt_) { post_event(EvEVApproaching()); return; }

		std::cout << "***" << std::endl << "LightState_5" << std::endl << "North-South: Red" << std::endl << "East-West: Yellow" << std::endl;

		std::this_thread::sleep_for(TIMER);

		std::cout << " *EVENT* " << "PostEvent EvToRR" << std::endl;
		post_event(EvToRR());
	}

	//Exit
	~LightState_5()
	{
		context<Operational>().previous_state_ = LS_5;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvToRR, InitialRR>, sc::transition<EvEVApproaching, Emergency> > reactions;
};

/*
* NorthSouth: Red
* EastWest: RedYellow
*/
struct LightState_6 : sc::state<LightState_6, NormalExecution>
{
	//Entry
	LightState_6(my_context ctx) : my_base(ctx)
	{
		//Check for emergency vehicle approaching.
		if (context<Machine>().emergency_interrupt_) { post_event(EvEVApproaching()); return; }

		std::cout << "***" << std::endl << "LightState_6" << std::endl << "North-South: Red" << std::endl << "East-West: RedYellow" << std::endl;

		std::this_thread::sleep_for(TIMER);

		std::cout << " *EVENT* " << "PostEvent ToSL4" << std::endl;
		post_event(EvToLS4());
	}

	//Exit
	~LightState_6()
	{
		context<Operational>().previous_state_ = LS_6;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvToLS4, LightState_4>, sc::transition<EvEVApproaching, Emergency> > reactions;
};


/*
Tier-III Emergency states
*/

struct EM_LightState_1;
struct EM_LightState_2;
struct EM_LightState_3;
struct EM_LightState_4;
struct EM_InitialRR;


struct EM_LightState_1 : sc::state<EM_LightState_1, Emergency>
{
	//Entry
	EM_LightState_1(my_context ctx) : my_base(ctx)
	{
		std::cout << "***" << std::endl << "Emergency LightState_1" << std::endl << "North-South: Green" << std::endl << "East-West: Red" << std::endl;
		
		std::this_thread::sleep_for(EM_TIMER);
		post_event(EvEVPassed());
	}

	//Exit
	~EM_LightState_1()
	{
		context<Operational>().previous_state_ = LightStates::LS_1;
	}

	//Events
	typedef sc::transition<EvEVPassed, NormalExecution> reactions;

};

struct EM_LightState_2 : sc::state<EM_LightState_2, Emergency>
{
	//Entry
	EM_LightState_2(my_context ctx) : my_base(ctx)
	{
		std::cout << "***" << std::endl << "Emergency LightState_2" << std::endl << "North-South: Red" << std::endl << "East-West: Yellow" << std::endl;
	}

	//Exit
	~EM_LightState_2()
	{
	}

	//Events
	typedef sc::custom_reaction<EvEmS2> reactions;

	sc::result react(const EvEmS2& e)
	{
		post_event(EvEmRR(e.navigate_to_));
		return transit<EM_InitialRR>();
	}
};

struct EM_LightState_3 : sc::state<EM_LightState_3, Emergency>
{
	//Entry
	EM_LightState_3(my_context ctx) : my_base(ctx)
	{
		std::cout << "***" << std::endl << "Emergency LightState_3" << std::endl << "North-South: Yellow" << std::endl << "East-West: Red" << std::endl;
	}

	//Exit
	~EM_LightState_3()
	{
	}

	//Events
	typedef sc::custom_reaction<EvEmS3> reactions;


	sc::result react(const EvEmS3& e)
	{
		post_event(EvEmRR(e.navigate_to_));
		return transit<EM_InitialRR>();
	}
};

struct EM_LightState_4 : sc::state<EM_LightState_4, Emergency>
{
	//Entry
	EM_LightState_4(my_context ctx) : my_base(ctx)
	{
		std::cout << "***" << std::endl << "Emergency LightState_4" << std::endl << "North-South: Red" << std::endl << "East-West: Green" << std::endl;
		
		std::this_thread::sleep_for(EM_TIMER);
		post_event(EvEVPassed());
	}

	//Exit
	~EM_LightState_4()
	{
		context<Operational>().previous_state_ = LightStates::LS_4;
	}

	//Events
	typedef sc::transition<EvEVPassed, NormalExecution> reactions;
};

struct EM_InitialRR : sc::state<EM_InitialRR, Emergency>
{
	//Entry
	EM_InitialRR(my_context ctx) : my_base(ctx)
	{
		std::cout << "***" << std::endl << "Emergency initialRR" << std::endl << "North-South: Red" << std::endl << "East-West: Red" << std::endl;
	}

	//Exit
	~EM_InitialRR()
	{
	}

	//Events
	typedef boost::mpl::list<sc::custom_reaction<EvEmRR>, sc::transition<EvEmS1, EM_LightState_1>, sc::transition<EvEmS4, EM_LightState_4> > reactions;

	sc::result react(const EvEmRR& e)
	{
		if (e.navigate_to_ == North || e.navigate_to_ == South)
		{
			post_event(EvEmS1());
		}
		else
		{
			post_event(EvEmS4());
		}

		return discard_event();
	}
};

struct EM_Choice : sc::state<EM_Choice, Emergency>
{
	//Entry
	EM_Choice(my_context ctx) : my_base(ctx)
	{
		std::cout << "***" << std::endl << "Current LightState is: " << context<Operational>().previous_state_ << std::endl;
		std::cout << "***" << std::endl << "Emergency Vehicle going: " << context<Machine>().ev_direction_ << std::endl;

		LightStates current = context<Operational>().previous_state_;
		Direction current_direction = context<Machine>().ev_direction_;

		switch (current)
		{
		case LightStates::LS_1:
		{
			post_event(EvEmS2(current_direction));
			break;
		}
		case LightStates::LS_4:
		{
			post_event(EvEmS3(current_direction));
			break;
		}
		default:
		{
			post_event(EvEmRR(current_direction));
			break;
		}
		}
	}

	//Exit
	~EM_Choice()
	{
		
	}

	//Events
	typedef boost::mpl::list<sc::custom_reaction<EvEmS2>, sc::custom_reaction<EvEmS3>, sc::custom_reaction<EvEmRR> > reactions;

	sc::result react(const EvEmS2& e)
	{
		post_event(e);
		return transit<EM_LightState_2>();
	}

	sc::result react(const EvEmS3& e)
	{
		post_event(e);
		return transit<EM_LightState_3>();
	}

	sc::result react(const EvEmRR& e)
	{
		post_event(e);
		return transit<EM_InitialRR>();
	}
};

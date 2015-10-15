#ifndef STATEMACHINE_CPP
#define STATEMACHINE_CPP

#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <iostream>
#include <thread>
#include "../include/SCUtils.h"
#include "../include/Log.h"

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

/*MACROS*/
#define TIMER std::chrono::seconds(3)
#define EM_TIMER std::chrono::seconds(5)

#define NE_NS_Green_EW_Red LightState_1 : sc::state<LightState_1, NormalExecution>
#define NE_NS_Yellow_EW_Red LightState_2 : sc::state<LightState_2, NormalExecution> 
#define NE_NS_RedYellow_EW_Red LightState_3 : sc::state<LightState_3, NormalExecution>
#define NE_NS_Red_EW_Green LightState_4 : sc::state<LightState_4, NormalExecution>
#define NE_NS_Red_EW_Yellow LightState_5 : sc::state<LightState_5, NormalExecution>
#define NE_NS_Red_EW_RedYellow LightState_6 : sc::state<LightState_6, NormalExecution>
#define NE_NS_Red_EW_Red InitialRR : sc::state<InitialRR, NormalExecution>

#define EM_NS_Green_EW_Red EM_LightState_1 : sc::state<EM_LightState_1, Emergency>
#define EM_NS_Red_EW_Yellow EM_LightState_2 : sc::state<EM_LightState_2, Emergency>
#define EM_NS_Yellow_EW_Red EM_LightState_3 : sc::state<EM_LightState_3, Emergency>
#define EM_NS_Red_EW_Green EM_LightState_4 : sc::state<EM_LightState_4, Emergency>
#define EM_NS_Red_EW_Red EM_InitialRR : sc::state<EM_InitialRR, Emergency>
#define EM_INITIAL EM_Choice : sc::state<EM_Choice, Emergency>

#define LOG_STATE(MESSAGE) {									\
LogInfo li;														\
li.message = MESSAGE;											\
boost::variant<LogTraffic, LogInfo, LogError> StateLogVariant;  \
StateLogVariant = li;											\
boost::apply_visitor(LogMessage(), StateLogVariant);			\
}																\

#define LOG_ERROR(MESSAGE) {									\
LogError li;													\
li.message = MESSAGE;											\
boost::variant<LogTraffic, LogInfo, LogError> StateLogVariant;  \
StateLogVariant = li;											\
boost::apply_visitor(LogMessage(), StateLogVariant);			\
}																\


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

struct EvToEmS1 : sc::event<EvToEmS1> {};

struct EvToEmS2 : sc::event<EvToEmS2>
{
	EvToEmS2(Direction ev_direction) : navigate_to_(ev_direction)
	{}

	Direction navigate_to_;
};

struct EvToEmS3 : sc::event<EvToEmS3>
{
	EvToEmS3(Direction ev_direction) : navigate_to_(ev_direction)
	{}

	Direction navigate_to_;
};

struct EvToEmS4 : sc::event<EvToEmS4> {};

struct EvToEmRR : sc::event<EvToEmRR>
{
	EvToEmRR(Direction ev_direction) : navigate_to_(ev_direction)
	{}

	Direction navigate_to_;
};

struct EvEVApproaching : sc::event<EvEVApproaching> {};
struct EvEVPassed : sc::event<EvEVPassed> {};

struct EvError : sc::event<EvError> {};

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

	//SETTERS
	void set_emergency_interrupt(Direction direction)
	{
		emergency_interrupt_ = true;
		ev_direction_ = direction;
	}

	void set_emergency_val(bool val)
	{
		emergency_interrupt_ = val;
	}

	void toggle_error_interrupt()
	{
		error_interupt_ = !error_interupt_;
	}

	void stop_execution()
	{
		stop_execution_ = true;
	}

	//GETTERS
	bool get_emergency_val()
	{
		return emergency_interrupt_;
	}

	bool get_error_val()
	{
		return error_interupt_;
	}

	bool get_execution_val()
	{
		return stop_execution_;
	}

	Direction get_direction()
	{
		return ev_direction_;
	}

private:
	bool emergency_interrupt_ = false;
	bool error_interupt_ = false;
	bool stop_execution_ = false;
	std::mutex interrupt_mutex_;
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
	virtual ~Operational()
	{

	}

	LightStates previous_state_;
};

struct YellowFlashing : sc::state<YellowFlashing, Machine>
{
	//Entry
	YellowFlashing(my_context ctx) : my_base(ctx)
	{
		LOG_ERROR("Error state entered.\nAn unknown error has occured, intersection offline.")

		while (context<Machine>().get_error_val() && !(context<Machine>().get_execution_val()));

		post_event(EvError());
	}

	//Exit
	virtual ~YellowFlashing()
	{
	}

	//Events
	typedef sc::transition<EvError, NormalExecution> reactions;
};


/*
Tier-II states
*/
struct InitialRR;
struct EM_Choice;

struct NormalExecution : sc::simple_state<NormalExecution, Operational, InitialRR>
{
	//Entry
	NormalExecution()
	{
	}

	//Exit
	virtual ~NormalExecution()
	{

	}

	//Events
};

struct Emergency : sc::state<Emergency, Operational, EM_Choice>
{
	//Entry
	Emergency(my_context ctx) : my_base(ctx)
	{
		//std::cout << "***" << std::endl << "Entering emergency state!" << std::endl << "***" << std::endl;
		LOG_STATE("Entering emegency state!")
	}

	//Exit
	virtual ~Emergency()
	{
		LOG_STATE("Leaving emergency state!")
		context<Machine>().set_emergency_val(false);
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
struct NE_NS_Red_EW_Red
{
	//Entry
	InitialRR(my_context ctx) : my_base(ctx)
	{
		//Check for emergency vehicle approaching.
		if (context<Machine>().get_execution_val()) { return; }
		if (context<Machine>().get_emergency_val()) { post_event(EvEVApproaching()); return; }
		if (context<Machine>().get_error_val()) { post_event(EvError()); return; }

		LOG_STATE("\n-------\nInitial state.\nNORTH - SOUTH: Red\nEAST - WEST : Red")

			std::this_thread::sleep_for(TIMER);

		//Nort-SOUTH have had green, thus its EAST-WEST's turn
		if (context<Operational>().previous_state_ == LS_2 || context<Operational>().previous_state_ == NORMALEXECUTION)
		{
			post_event(EvToLS6());
		}
		else
		{
			post_event(EvToLS3());
		}
	}

	//Exit
	virtual ~InitialRR()
	{
		context<Operational>().previous_state_ = INITIALRR;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvToLS6, LightState_6>, sc::transition<EvToLS3, LightState_3>, sc::transition<EvEVApproaching, Emergency>, sc::transition<EvError, YellowFlashing> > reactions;
};

/*
* NorthSouth: Green
* EastWest: Red
*/
struct NE_NS_Green_EW_Red
{
	//Entry
	LightState_1(my_context ctx) : my_base(ctx)
	{
		//Check for emergency vehicle approaching.
		if (context<Machine>().get_emergency_val()) { post_event(EvEVApproaching()); return; }
		if (context<Machine>().get_error_val()) { post_event(EvError()); return; }

		LOG_STATE("\n-------\nLightState 1\nNORTH-SOUTH: Green\nEAST-WEST: Red")

			std::this_thread::sleep_for(TIMER);

		post_event(EvToLS2());
	}

	//Exit
	virtual ~LightState_1()
	{
		context<Operational>().previous_state_ = LS_1;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvToLS2, LightState_2>, sc::transition<EvEVApproaching, Emergency>, sc::transition<EvError, YellowFlashing> > reactions;
};

/*
* NorthSouth: Yellow
* EastWest: Red
*/
struct NE_NS_Yellow_EW_Red
{
	//Entry
	LightState_2(my_context ctx) : my_base(ctx)
	{
		//Check for emergency vehicle approaching.
		if (context<Machine>().get_emergency_val()) { post_event(EvEVApproaching()); return; }
		if (context<Machine>().get_error_val()) { post_event(EvError()); return; }

		LOG_STATE("\n-------\nLightState 2\nNORTH-SOUTH: Yellow\nEAST-WEST: Red")

			std::this_thread::sleep_for(TIMER);

		post_event(EvToRR());
	}

	//Exit
	virtual ~LightState_2()
	{
		context<Operational>().previous_state_ = LS_2;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvToRR, InitialRR>, sc::transition<EvEVApproaching, Emergency>, sc::transition<EvError, YellowFlashing> > reactions;
};

/*
* NorthSouth: RedYellow
* EastWest: Red
*/
struct NE_NS_RedYellow_EW_Red
{
	//Entry
	LightState_3(my_context ctx) : my_base(ctx)
	{
		//Check for emergency vehicle approaching.
		if (context<Machine>().get_emergency_val()) { post_event(EvEVApproaching()); return; }
		if (context<Machine>().get_error_val()) { post_event(EvError()); return; }

		LOG_STATE("\n-------\nLightState 3\nNORTH-SOUTH: RedYellow\nEAST-WEST: Red")

			std::this_thread::sleep_for(TIMER);

		post_event(EvToLS1());
	}

	//Exit
	virtual ~LightState_3()
	{
		context<Operational>().previous_state_ = LS_3;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvToLS1, LightState_1>, sc::transition<EvEVApproaching, Emergency>, sc::transition<EvError, YellowFlashing> > reactions;
};

/*
* NorthSouth: Red
* EastWest: Green
*/
struct NE_NS_Red_EW_Green
{
	//Entry
	LightState_4(my_context ctx) : my_base(ctx)
	{
		//Check for emergency vehicle approaching.
		if (context<Machine>().get_emergency_val()) { post_event(EvEVApproaching()); return; }
		if (context<Machine>().get_error_val()) { post_event(EvError()); return; }

		LOG_STATE("\n-------\nLightState 4\nNORTH-SOUTH: Red\nEAST-WEST: Green")

			std::this_thread::sleep_for(TIMER);

		post_event(EvToLS5());
	}

	//Exit
	virtual ~LightState_4()
	{
		context<Operational>().previous_state_ = LS_4;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvToLS5, LightState_5>, sc::transition<EvEVApproaching, Emergency>, sc::transition<EvError, YellowFlashing> >  reactions;
};

/*
* NorthSouth: Red
* EastWest: Yellow
*/
struct NE_NS_Red_EW_Yellow
{
	//Entry
	LightState_5(my_context ctx) : my_base(ctx)
	{
		//Check for emergency vehicle approaching.
		if (context<Machine>().get_emergency_val()) { post_event(EvEVApproaching()); return; }
		if (context<Machine>().get_error_val()) { post_event(EvError()); return; }

		LOG_STATE("\n-------\nLightState 5\nNORTH-SOUTH: Red\nEAST-WEST: Yellow")

			std::this_thread::sleep_for(TIMER);

		post_event(EvToRR());
	}

	//Exit
	virtual ~LightState_5()
	{
		context<Operational>().previous_state_ = LS_5;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvToRR, InitialRR>, sc::transition<EvEVApproaching, Emergency>, sc::transition<EvError, YellowFlashing> > reactions;
};

/*
* NorthSouth: Red
* EastWest: RedYellow
*/
struct NE_NS_Red_EW_RedYellow
{
	//Entry
	LightState_6(my_context ctx) : my_base(ctx)
	{
		//Check for emergency vehicle approaching.
		if (context<Machine>().get_emergency_val()) { post_event(EvEVApproaching()); return; }
		if (context<Machine>().get_error_val()) { post_event(EvError()); return; }

		LOG_STATE("\n-------\nLightState 6\nNORTH-SOUTH: Red\nEAST-WEST: RedYellow")

			std::this_thread::sleep_for(TIMER);

		post_event(EvToLS4());
	}

	//Exit
	virtual ~LightState_6()
	{
		context<Operational>().previous_state_ = LS_6;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvToLS4, LightState_4>, sc::transition<EvEVApproaching, Emergency>, sc::transition<EvError, YellowFlashing> > reactions;
};


/*
Tier-III Emergency states
*/

struct EM_LightState_1;
struct EM_LightState_2;
struct EM_LightState_3;
struct EM_LightState_4;
struct EM_InitialRR;


struct EM_NS_Green_EW_Red
{
	//Entry
	EM_LightState_1(my_context ctx) : my_base(ctx)
	{
		if (context<Machine>().get_error_val()) { post_event(EvError()); return; }
		LOG_STATE("\n-------\nEmergency LightState 1\nNORTH-SOUTH: Green\nEAST-WEST: Red")

			std::this_thread::sleep_for(EM_TIMER);
		post_event(EvEVPassed());
	}

	//Exit
	virtual ~EM_LightState_1()
	{
		context<Operational>().previous_state_ = LightStates::LS_1;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvEVPassed, NormalExecution>, sc::transition<EvError, YellowFlashing> > reactions;

};

struct EM_NS_Red_EW_Yellow
{
	//Entry
	EM_LightState_2(my_context ctx) : my_base(ctx)
	{
		if (context<Machine>().get_error_val()) { post_event(EvError()); return; }
		LOG_STATE("\n-------\nEmergency LightState 2\nNORTH-SOUTH: Red\nEAST-WEST: Yellow")
			TIMER;
	}

	//Exit
	virtual ~EM_LightState_2()
	{
	}

	//Events
	typedef boost::mpl::list<sc::custom_reaction<EvToEmS2>, sc::transition<EvError, YellowFlashing> > reactions;

	sc::result react(const EvToEmS2& e)
	{
		post_event(EvToEmRR(e.navigate_to_));
		return transit<EM_InitialRR>();
	}
};

struct EM_NS_Yellow_EW_Red
{
	//Entry
	EM_LightState_3(my_context ctx) : my_base(ctx)
	{
		if (context<Machine>().get_error_val()) { post_event(EvError()); return; }
		LOG_STATE("\n-------\nEmergency LightState 3\nNORTH-SOUTH: Yellow\nEAST-WEST: Red")
			TIMER;
	}

	//Exit
	virtual ~EM_LightState_3()
	{
	}

	//Events
	typedef boost::mpl::list<sc::custom_reaction<EvToEmS3>, sc::transition<EvError, YellowFlashing> > reactions;


	sc::result react(const EvToEmS3& e)
	{
		post_event(EvToEmRR(e.navigate_to_));
		return transit<EM_InitialRR>();
	}
};

struct EM_NS_Red_EW_Green
{
	//Entry
	EM_LightState_4(my_context ctx) : my_base(ctx)
	{
		if (context<Machine>().get_error_val()) { post_event(EvError()); return; }
		LOG_STATE("\n-------\nEmergency LightState 4\nNORTH-SOUTH: Red\nEAST-WEST: Green")

			std::this_thread::sleep_for(EM_TIMER);
		post_event(EvEVPassed());
	}

	//Exit
	virtual ~EM_LightState_4()
	{
		context<Operational>().previous_state_ = LightStates::LS_4;
	}

	//Events
	typedef boost::mpl::list<sc::transition<EvEVPassed, NormalExecution>, sc::transition<EvError, YellowFlashing> > reactions;
};

struct EM_NS_Red_EW_Red
{
	//Entry
	EM_InitialRR(my_context ctx) : my_base(ctx)
	{
		if (context<Machine>().get_execution_val()) { return; }
		if (context<Machine>().get_error_val()) { post_event(EvError()); return; }
		LOG_STATE("\n-------\nEmergency InitialRR\nNORTH-SOUTH: Red\nEAST-WEST: Red")
			TIMER;
	}

	//Exit
	virtual ~EM_InitialRR()
	{
	}

	//Events
	typedef boost::mpl::list<sc::custom_reaction<EvToEmRR>, sc::transition<EvToEmS1, EM_LightState_1>, sc::transition<EvToEmS4, EM_LightState_4>, sc::transition<EvError, YellowFlashing> > reactions;

	sc::result react(const EvToEmRR& e)
	{
		if (e.navigate_to_ == NORTH || e.navigate_to_ == SOUTH)
		{
			post_event(EvToEmS1());
		}
		else
		{
			post_event(EvToEmS4());
		}

		return discard_event();
	}
};

struct EM_INITIAL
{
	//Entry
	EM_Choice(my_context ctx) : my_base(ctx)
	{
		//std::cout << "***" << std::endl << "Current LightState is: " << context<Operational>().previous_state_ << std::endl;
		//std::cout << "***" << std::endl << "Emergency Vehicle going: " << context<Machine>().ev_direction_ << std::endl;

		if (context<Machine>().get_error_val()) { post_event(EvError()); return; }
		TIMER;
		LightStates current = context<Operational>().previous_state_;
		Direction current_direction = context<Machine>().get_direction();

		switch (current)
		{
		case LightStates::LS_1:
		{
			post_event(EvToEmS2(current_direction));
			break;
		}
		case LightStates::LS_4:
		{
			post_event(EvToEmS3(current_direction));
			break;
		}
		default:
		{
			post_event(EvToEmRR(current_direction));
			break;
		}
		}
	}

	//Exit
	virtual ~EM_Choice()
	{

	}

	//Events
	typedef boost::mpl::list<sc::custom_reaction<EvToEmS2>, sc::custom_reaction<EvToEmS3>, sc::custom_reaction<EvToEmRR>, sc::transition<EvError, YellowFlashing> > reactions;

	sc::result react(const EvToEmS2& e)
	{
		post_event(e);
		return transit<EM_LightState_2>();
	}

	sc::result react(const EvToEmS3& e)
	{
		post_event(e);
		return transit<EM_LightState_3>();
	}

	sc::result react(const EvToEmRR& e)
	{
		post_event(e);
		return transit<EM_InitialRR>();
	}
};

#endif
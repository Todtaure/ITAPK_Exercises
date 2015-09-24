#pragma once
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>

namespace sc = boost::statechart;

/*
State translations:
LightState_1	NS-Green, EW-Red
LightState_2	NS-Yellow, EW-Red
InitialRR		NS-Red, EW-Red
LightState_4	NS-RedYellow, EW-Red
LightState_5	NS-Red, EW-Green
LightState_6	NS-Red, EW-Yellow
LightState_7	NS-Red, EW-RedYellow

*/

/*
Events
*/



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

	}

	//Exit
	~NormalExecution()
	{

	}
};

struct Emergency : sc::simple_state<Emergency, Operational, InitialRR_Emergency>
{
	//Entry
	Emergency()
	{

	}

	//Exit
	~Emergency()
	{

	}
};

/*
Tier-III states
*/

struct InitialRR : sc::simple_state<InitialRR, boost::mpl::list<NormalExecution, Emergency>>
{
	//Entry
	InitialRR()
	{
		
	}

	//Exit
	~InitialRR()
	{
		
	}

	//Events


};

struct LightState_1 : sc::simple_state<LightState_1, boost::mpl::list<NormalExecution, Emergency> >
{
	//Entry
	LightState_1()
	{

	}

	//Exit
	~LightState_1()
	{

	}
};

struct LightState_2 : sc::simple_state<LightState_2, boost::mpl::list<NormalExecution, Emergency>>
{
	//Entry
	LightState_2()
	{

	}

	//Exit
	~LightState_2()
	{

	}
};

struct LightState_4 : sc::simple_state<LightState_4, boost::mpl::list<NormalExecution, Emergency>>
{
	//Entry
	LightState_4()
	{

	}

	//Exit
	~LightState_4()
	{

	}
};

struct LightState_5 : sc::simple_state<LightState_5, boost::mpl::list<NormalExecution, Emergency>>
{
	//Entry
	LightState_5()
	{

	}

	//Exit
	~LightState_5()
	{

	}
};

struct LightState_6 : sc::simple_state<LightState_6, boost::mpl::list<NormalExecution, Emergency>>
{
	//Entry
	LightState_6()
	{

	}

	//Exit
	~LightState_6()
	{

	}
};

struct LightState_7 : sc::simple_state<LightState_7, boost::mpl::list<NormalExecution, Emergency>>
{
	//Entry
	LightState_7()
	{

	}

	//Exit
	~LightState_7()
	{

	}
};


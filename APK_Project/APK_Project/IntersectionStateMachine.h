#pragma once
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>

namespace sc = boost::statechart;

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
	
};


/*
Tier-I states
*/

struct NormalExecution;
struct Emergency;

struct Operational : sc::simple_state<Operational, Machine, NormalExecution>
{
	
};

struct YellowFlashing : sc::simple_state<YellowFlashing, Machine>
{
	
};


/*
Tier-II states
*/
struct InitialRR;
struct InitialRR_Emergency;

struct NormalExecution : sc::simple_state<NormalExecution, Operational, InitialRR>
{
	
};

struct Emergency : sc::simple_state<Emergency, Operational, InitialRR_Emergency>
{
	
};

/*
Tier-III states
*/

struct NSG : sc::simple_state<NSG, NormalExecution>
{
	
};

struct NSY : sc::simple_state<NSY, NormalExecution>
{
	
};

struct NSR : sc::simple_state<NSR, NormalExecution>
{
	
};

struct NSRY : sc::simple_state<NSRY, NormalExecution>
{
	
};

struct EWG : sc::simple_state<EWG, NormalExecution>
{
	
};

struct EWY : sc::simple_state<EWY, NormalExecution>
{
	
};

struct EWR : sc::simple_state<EWR, NormalExecution>
{
	
};

struct EWRY : sc::simple_state<EWRY, NormalExecution>
{
	
};

struct InitialRR : sc::simple_state<InitialRR, NormalExecution>
{
	
};

//Emergency states


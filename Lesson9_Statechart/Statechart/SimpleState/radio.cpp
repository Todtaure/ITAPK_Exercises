/*****************************************/
/* Copyright: DevelEdu 2013              */
/* Author: sha                           */
/*****************************************/

#include <iostream>
#include <string>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>



/**
 * Helpers
 * Makes it quick and easy to create entry and exit actions
 * -> constructor and destructor and have them write something to cout.
 *
 * Usage
 * struct Test
 * {
 *    PRINT_ENTRY_EXIT(0, Test)
 * };
 * 
 * Note the missing ;
 * param 1 - Is state inner level - denoted on printout with '*'
 * param 2 - Is the name of the class in question. No quotes!
 */
void print(unsigned int lvl, const std::string& name)
{
  for(unsigned int i = 0; i< lvl; ++i)
    std::cout << "*";
  std::cout << " " << name << std::endl;
}

#define PRINT_ENTRY_EXIT(lvl, name)             \
  name() { print(lvl, #name"()"); }             \
  ~name() { print(lvl, "~"#name"()"); }



namespace sc = boost::statechart;


/**
 * Events that can be handled
 */

struct EvAMTuner : sc::event<EvAMTuner>{};

struct EvFMTuner : sc::event<EvFMTuner>{};


/**
 * Machine definition and the top level states
 * forward declarations
 */

struct FMTuner;

struct AMTuner;

struct Machine : sc::state_machine<Machine, FMTuner>{	};

/**
 * FMTuner
 */

struct FMTuner : sc::simple_state<FMTuner,Machine>
{
	PRINT_ENTRY_EXIT(0, FMTuner)
	typedef sc::transition<EvAMTuner, AMTuner> reactions;
};

/**
 * AMTuner
 */

struct AMTuner : sc::simple_state<AMTuner, Machine>
{
	PRINT_ENTRY_EXIT(0, AMTuner)
	typedef sc::transition<EvFMTuner, FMTuner> reactions;
};

int main()
{
  /* Instantiate State machine and make it process some relevant events... */
  
	Machine my_machine;
	my_machine.initiate();

	my_machine.process_event(EvAMTuner());

	my_machine.process_event(EvFMTuner());

	getchar();
	return 0;
}

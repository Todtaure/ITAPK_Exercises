/*****************************************/
/* Copyright: DevelEdu 2013              */
/* Author: sha                           */
/*****************************************/

#include <iostream>
#include <string>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/deep_history.hpp>
#include <boost/statechart/shallow_history.hpp>


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
struct EvOn : sc::event<EvOn> {};
struct EvOff : sc::event<EvOff> {};

struct EvAMTuner : sc::event<EvAMTuner> {};
struct EvFMTuner : sc::event<EvFMTuner> {};
  

/**
 * Machine definition and the top level states
 * forward declarations
 */
struct On;
struct Off;
struct Machine : sc::state_machine<Machine, Off> {};


/**
 * Off
 */
struct Off : sc::simple_state<Off, Machine>
{
  typedef sc::transition<EvOn, On>   reactions;
  PRINT_ENTRY_EXIT(0, Off)
};


/**
 * 'On' and sub states
 * Forward declarations comes first...
 */
struct RadioPlaying;

/* Missing 'On' */

struct On : sc::simple_state<On, Machine, RadioPlaying>
{
	typedef sc::transition<EvOff, Off> reactions;
	PRINT_ENTRY_EXIT(0, On)
};

/**
 * 'RadioPlaying' and sub states
 * Forward declarations comes first...
 */
struct FMTuner;
struct AMTuner;

/* Missing 'RadioPlaying' */

struct RadioPlaying : sc::simple_state<RadioPlaying, On, FMTuner>
{
	PRINT_ENTRY_EXIT(0, RadioPlaying);
};

/**
 * FMTuner
 */
struct FMTuner : sc::simple_state<FMTuner, RadioPlaying>
{
  typedef sc::transition<EvAMTuner, AMTuner> reactions;
  PRINT_ENTRY_EXIT(1, FMTuner);
};


/**
 * AMTuner
 */
struct AMTuner : sc::simple_state<AMTuner, RadioPlaying>
{
  typedef sc::transition<EvFMTuner, FMTuner> reactions;
  PRINT_ENTRY_EXIT(1, AMTuner);
};



int main()
{
  Machine myMachine;  
  myMachine.initiate();

  myMachine.process_event(EvOn());
  myMachine.process_event(EvAMTuner());
  myMachine.process_event(EvOff());
  
  getchar();
  return 0;
}

#include <iostream>
#include "Events.hpp"
#include "Timer.hpp"
#include <memory>


Timer::Timer( int timer) :
eventTimer_(timer), terminator_(false), next_id(0)
{
	thread_     = new std::thread( std::bind( &Timer::timerThreadFunction, this ) );
}

int Timer::attach(std::function<void(const std::shared_ptr<Event>&)> cb)
{
	cb_map.insert(std::pair<int, std::function<void(const std::shared_ptr<Event>&)> >(next_id, cb));
	return next_id++;
}

void Timer::detach(int id)
{
	cb_map.erase(id);
}

Timer::~Timer()
{
	terminator_ = true;
	thread_->join();
	delete thread_;
}

int Timer::timerThreadFunction()
{
  int counter = 0;
  while( !terminator_ )
  {
    std::chrono::milliseconds sleepFor( 1000 );
    std::this_thread::sleep_for( sleepFor );

    notifyAll( std::make_shared<Event>() );
    if( ++counter >= eventTimer_ )
    {	
      counter = 0;
      std::cout << "EventOther created..." << std::endl;
      
      notifyAll( std::make_shared<EventOther>() );
    }
  }
  return 0;
}

void Timer::notifyAll(const std::shared_ptr<Event>& ev)
{
	
	std::for_each(cb_map.begin(), cb_map.end(), [ev](std::pair<int, std::function<void(const std::shared_ptr<Event>&)> > cb_pair)
	{
		cb_pair.second(ev);
	});
}
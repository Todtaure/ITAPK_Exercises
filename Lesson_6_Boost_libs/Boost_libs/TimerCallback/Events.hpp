#ifndef _EVENTS_HPP_
#define _EVENTS_HPP_

#include <chrono>

/**
 * Event
 */
class Event
{
public:
  Event()
    : creationTime_(std::chrono::system_clock::now())
  {
  }
  std::chrono::time_point<std::chrono::system_clock> creationTime() const
  {
    return creationTime_;
  }
  
private:
  std::chrono::time_point<std::chrono::system_clock> creationTime_;
};


/**
 * EventOther
 */
class EventOther : public Event {};

std::ostream& operator<<(std::ostream& os, const Event& ev);


#endif

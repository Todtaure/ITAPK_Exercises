#ifndef TIMER_HPP_
#define TIMER_HPP_
#include <string>
#include <map>
#include <thread>
#include <functional>
#include <chrono>
#include <boost/any.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

class Timer
{
public:
  explicit Timer( int timer = 10);
  int attach(std::function<void(const std::shared_ptr<Event>&)>);
  void detach(int);
  ~Timer();  
  
private:
  int  timerThreadFunction();
  void notifyAll(const std::shared_ptr<Event> &);

  int           eventTimer_;
  std::thread*	thread_;
  bool          terminator_;
  std::map<int, std::function<void(const std::shared_ptr<Event>&)> > cb_map;
  int next_id;
};






#endif

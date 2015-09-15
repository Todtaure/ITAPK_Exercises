/*****************************************/
/* Copyright: DevelEdu 2013              */
/* Author: sha                           */
/*****************************************/

// bindanyfunction.cpp : Defines the entry point for the console application.
//

#include <string>
#include <iostream>
#include <mutex>
#include <functional>
#include "Events.hpp"
#include "Timer.hpp"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/core/ref.hpp>


std::mutex stdioProt;
using namespace std::placeholders;


void freeFunction( const std::shared_ptr<Event>& event )
{
   // Protecting stdio since its not thread safe
  std::lock_guard<std::mutex> lock(stdioProt );

  std::cout << "free function event: " << event << std::endl;
  
}

struct cbFunct
{
	void operator()(const std::shared_ptr<Event>& event)
	{
		std::cout << "func event: " << event << std::endl;
	}
};



void withAnExtra(const std::shared_ptr<Event>& event, const std::string text)
{
   // Protecting stdio since its not thread safe
  std::lock_guard<std::mutex> lock(stdioProt );

  std::cout << event << " and text: " << text << std::endl;

}


class ReferenceObj
{
public:
   ReferenceObj() : called_(0) 
   {
   }

  void call( const std::shared_ptr<Event>& event )
  {
    // Protecting stdio since its not thread safe
    std::lock_guard<std::mutex> lock(stdioProt );

	std::cout << "called: " << called_ << " event: " << event << std::endl;
    
    ++called_;
  }

   int gotCalled()
   {
      return called_;
   }
  
private:
   int   called_;
};



int main()
{
  // Try to make several timers with different callbacks
  Timer t1( 4 );
  t1.attach( freeFunction );
  t1.attach(cbFunct());
  t1.attach(std::bind(withAnExtra, _1, "something"));

  // The threads run naturally in the background (no clean up has been added for the threads)
  ReferenceObj reference_obj;

  t1.attach(std::bind(&ReferenceObj::call, std::ref(reference_obj), _1));
   
  // You might wanna change the loop below such that you can extract the value of called_
  //  from your instance of ReferenceObj
  for(;;)
  {
    std::chrono::milliseconds sleepFor(1000);
    std::this_thread::sleep_for(sleepFor);
  }
   

  return EXIT_SUCCESS;
}

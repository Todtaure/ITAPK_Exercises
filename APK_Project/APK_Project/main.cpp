#include "IntersectionStateMachine.cpp"
#include <future>
#include <stdexcept>
#include <exception>
#include <sstream>
#include <boost/ref.hpp>

class InterruptException : public std::runtime_error
{
public:
	InterruptException() : runtime_error("Statemachine interrupted.")
	{}

	virtual const char * what() const throw() override
	{
		return runtime_error::what();
	}

};

std::mutex interrupt_mutex;

bool query_interrupt(Machine *m)
{
	std::lock_guard<std::mutex> lock_guard(interrupt_mutex);
	return m->emergency_interrupt_;
}

void toggle_interrupt(Machine *m)
{
	std::lock_guard<std::mutex> lock_guard(interrupt_mutex);
	m->emergency_interrupt_ = !m->emergency_interrupt_;
}

void check_process_event(const boost::statechart::event_base &evt, Machine *m)
{
	if (query_interrupt(m))
	{
		throw InterruptException();
	}
	m->process_event(evt);
}

void run_normal_execution(Machine *m)
{
	while (true)
	{
		try {
			check_process_event(EvToLS6(), m);
			check_process_event(EvToLS4(), m);
			check_process_event(EvToLS5(), m);
			check_process_event(EvToRR(), m);
			check_process_event(EvToLS3(), m);
			check_process_event(EvToLS1(), m);
			check_process_event(EvToLS2(), m);
			check_process_event(EvToRR(), m);
		}
		catch (InterruptException ex)
		{
			std::cout << "Normal execution aborted. " << ex.what() << std::endl;
			return;
		}
	}
}


int main()
{
	Machine *m = new Machine();

	m->initiate();

	std::thread work1(run_normal_execution, m);

	std::this_thread::sleep_for(std::chrono::seconds(3));

	toggle_interrupt(m);
	std::cout << "Interrupt activated!" << std::endl;

	work1.join();

	m->process_event(EvEVApproaching());

	getchar();

	delete m;
	return 0;
}
#include "IntersectionStateMachine.cpp"
#include <future>
#include <stdexcept>
#include <exception>
#include <sstream>
#include <boost/ref.hpp>

int main()
{
	Machine *m = new Machine();
	std::thread work1(std::bind(&Machine::initiate, m));
	
	std::this_thread::sleep_for(std::chrono::seconds(2));

	m->set_emergency_interrupt(Direction::East);

	std::cout << "Interrupt activated!" << std::endl;

	getchar();
	
	work1.join();
	delete m;
	return 0;
}
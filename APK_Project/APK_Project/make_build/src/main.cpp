#pragma once
#include "IntersectionStateMachine.cpp"
#include "VehicleDetector.h"
#include "Log.h"
#include <sstream>
#include <thread>


int main()
{
	TrafficInfo tInfo;

	sigLog.connect(Logger<Log>());
	sigTotalTraffic.connect(Logger<LogTraffic>());
	sigTotalTraffic.connect(boost::ref(tInfo));

	Machine *m = new Machine();

	std::thread intersection_thread(std::bind(&Machine::initiate, m));
	std::thread vehicle_detector_thread(VehicleDetection);

	char in = '1';

	while (in != 'q')
	{
		std::cin >> in;
		switch (in)
		{
			case 'n':
			{
				m->set_emergency_interrupt(NORTH);
				break;
			}
			case 's':
			{
				m->set_emergency_interrupt(SOUTH);
				break;
			}
			case 'e':
			{
				m->set_emergency_interrupt(EAST);
				break;
			}
			case 'w':
			{
				m->set_emergency_interrupt(WEST);
				break;
			}
			case 'f':
			{
				m->toggle_error_interrupt();
				break;
			}
			case 'q':
			{
				m->stop_execution();
				break;
			}
		}
	}

	getchar();	
	intersection_thread.join();
	std::cout << "Statemachine execution has stopped." << std::endl;
	vehicle_detector_thread.join();
	
	delete m;

	return 0;
}
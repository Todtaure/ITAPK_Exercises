#ifndef MAIN_CPP
#define MAIN_CPP

#include "IntersectionStateMachine.cpp"
#include "../include/VehicleDetection.h"
#include "../include/Log.h"
#include "../include/SCUtils.h"
#include <sstream>
#include <thread>

using namespace SCUtils;

struct Controller
{
	void Run() const
	{	
		TrafficInfo tInfo;
		Machine *m = new Machine;
		VehicleDetection *vd = new VehicleDetection;

		sigLog.connect(Logger<Log>());
		sigTotalTraffic.connect(Logger<LogTraffic>());
		sigTotalTraffic.connect(boost::ref(tInfo));
		
		std::thread intersection_thread(std::bind(&Machine::initiate, m));
		std::thread vehicle_detector_thread(std::bind(&VehicleDetection::Run, vd));

		char in = 'g';

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
				vd->stop_execution();
				std::cout << "Waiting for StateMachine to stop.\nWaiting for VehicleDetection to stop." << std::endl;
				break;
			}
			}
		}

		getchar();

		intersection_thread.join();
		std::cout << "StateMachine execution has stopped." << std::endl;
		vehicle_detector_thread.join();
		std::cout << "VehicleDetection execution has stopped." << std::endl;

		delete m;
		delete vd;
	}
};

int main()
{
	
	Controller ctrl;
	ctrl.Run();
	return 0;
}

#endif

#pragma once
#include "IntersectionStateMachine.cpp"
#include "VehicleDetector.h"
#include "Log.h"
#include <sstream>


int main()
{			
	TrafficInfo tInfo;

	sigInfo.connect(Log<LogInfo>());
	sigError.connect(Log<LogError>());
	sigTotalTraffic.connect(Log<LogTraffic>());
	sigTotalTraffic.connect(boost::ref(tInfo));

	/*Machine *m = new Machine();
	std::thread work1(std::bind(&Machine::initiate, m));*/

	std::this_thread::sleep_for(std::chrono::seconds(1));

	//m->set_emergency_interrupt(Direction::EAST);

	std::thread VehicleDetectorThread(VehicleDetection);
	VehicleDetectorThread.join();
	
	getchar();

	//work1.join();
	//delete m;

	return 0;
}
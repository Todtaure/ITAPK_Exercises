#include "IntersectionStateMachine.cpp"
#include <future>
#include <stdexcept>
#include <exception>
#include <sstream>
#include <boost/ref.hpp>
#include <boost/bind/bind.hpp>
#include "Log.h"
#include "VehicleDetector.h"


int main()
{			
	sigInfo.connect(Log<LogInfo>());
	sigError.connect(Log<LogError>());
	sigTraffic.connect(Log<LogTraffic>());

	Machine *m = new Machine();
	std::thread work1(std::bind(&Machine::initiate, m));

	std::this_thread::sleep_for(std::chrono::seconds(2));

	m->set_emergency_interrupt(Direction::East);

	std::cout << "Interrupt activated!" << std::endl;

/*
	TrafficInfo test_traffic_info;
	sigTraffic.connect(boost::bind(&TrafficInfo::TrafficMilestone, boost::ref(test_traffic_info), _1));


	LogInfo testInfo;
	testInfo.message = "Green light";
	sigInfo(testInfo);

	LogError testError;
	testError.message = "Crash and burn";
	sigError(testError);
	
	LogTraffic testTraffic;
	testTraffic.direction = LogTraffic::NORTH;
	testTraffic.numberOfEmergencies = 1;
	testTraffic.numberOfVehicles = 142;
	sigTraffic(testTraffic);

	testTraffic.numberOfVehicles = 242;
	sigTraffic(testTraffic);*/
	

	std::thread VehicleDetectorThread(VehicleDetection);
	VehicleDetectorThread.join();
	
	getchar();

	work1.join();
	delete m;

	return 0;
}
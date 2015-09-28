#include "IntersectionStateMachine.cpp"
#include <future>
#include <boost/ref.hpp>
#include <boost/bind/bind.hpp>
#include "Log.h"

int main()
{	
	Machine intersection;
		
	sigInfo.connect(Log<LogInfo>());
	sigError.connect(Log<LogError>());
	sigTraffic.connect(Log<LogTraffic>());

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
	sigTraffic(testTraffic);

	
	
	getchar();

	return 0;
}
#ifndef VEHICLEDETECTOR_H
#define VEHICLEDETECTOR_H

#include "Log.h"
#include "SCUtils.h"
#include <thread>

inline void VehicleDetection()
{	
	LogTraffic NorthTraffic, SouthTraffic, WestTraffic, EastTraffic;

	std::list<LogTraffic> intersection(3);

	NorthTraffic.direction = SCUtils::Direction::NORTH;
	SouthTraffic.direction = SCUtils::Direction::SOUTH;
	WestTraffic.direction = SCUtils::Direction::WEST;
	EastTraffic.direction = SCUtils::Direction::EAST;

	while (true)
	{
		intersection.push_back(NorthTraffic);
		intersection.push_back(SouthTraffic);
		intersection.push_back(WestTraffic);
		intersection.push_back(EastTraffic);

		/*Foreach using Lambda expression*/
		/*std::for_each(intersection.begin(), intersection.end(), [](LogTraffic &ti)
		{
			ti.numberOfVehicles = rand() % 150 + 40;
			ti.numberOfEmergencies = rand() % 5;
		});*/

		/*For using auto*/
		for(auto &ti : intersection)
		{
			ti.numberOfVehicles = rand() % 150 + 40;
			ti.numberOfEmergencies = rand() % 5;
		}

		boost::variant<LogTraffic, LogInfo, LogError, std::list<LogTraffic> > LogVariant;
		LogVariant = intersection;

		/*Applying visitation method*/
		boost::apply_visitor(LogMessage(), LogVariant);

		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
}

#endif
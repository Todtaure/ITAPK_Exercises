#pragma once

#include <thread>

inline void VehicleDetection()
{	
	LogTraffic NorthTraffic;
	LogTraffic SouthTraffic;
	LogTraffic WestTraffic;
	LogTraffic EastTraffic;
	
	NorthTraffic.direction = LogTraffic::NORTH;
	SouthTraffic.direction = LogTraffic::SOUTH;
	WestTraffic.direction = LogTraffic::WEST;
	EastTraffic.direction = LogTraffic::EAST;

	while (true)
	{
		NorthTraffic.numberOfVehicles = rand() % 150 + 40;
		NorthTraffic.numberOfEmergencies = rand() % 5;

		SouthTraffic.numberOfVehicles = rand() % 150 + 40;
		SouthTraffic.numberOfEmergencies = rand() % 5;
		
		WestTraffic.numberOfVehicles = rand() % 150 + 40;
		WestTraffic.numberOfEmergencies = rand() % 5;

		EastTraffic.numberOfVehicles = rand() % 150 + 40;
		EastTraffic.numberOfEmergencies = rand() % 5;
			
		sigTraffic(NorthTraffic);
		sigTraffic(SouthTraffic);
		sigTraffic(WestTraffic);
		sigTraffic(EastTraffic);

		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
}
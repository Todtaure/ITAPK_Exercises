#pragma once
#include <iostream>
#include <boost/variant.hpp>
#include <boost/signals2/signal.hpp>
#include <fstream>
#include <sstream>

struct LogInfo
{
	std::string message;
};

struct LogError
{
	std::string message;
};

static const char* direction_names[] = { "NORTH", "SOUTH", "WEST", "EAST" };

struct LogTraffic
{
	enum direction
	{
		NORTH,
		SOUTH,
		WEST,
		EAST
	};

	direction direction;
	int numberOfVehicles;
	int numberOfEmergencies;
};

inline void WriteToFile(std::string path, std::vector<std::string> vec) // std::stringstream &ss
{
	std::mutex fileLock;
	try
	{
		fileLock.lock();
		std::fstream fs(path, std::fstream::out | std::fstream::app);

		std::ostream_iterator<std::string> output(fs);
		std::copy(vec.begin(), vec.end(), output);
		
		//std::ostream_iterator<std::string> output(fs);
		//fs << ss.rdbuf();

		fs.close();
		fileLock.unlock();
	}
	catch (...)
	{
		std::cout << "Couldn't write to file" << std::endl;
		fileLock.unlock();
	}
}

template<typename T, typename D>
struct Typelist
{
	typedef T First;
	typedef D Rest;
};

struct NullType{};
typedef Typelist<LogInfo, Typelist<LogError, NullType> > my_typelist;

template<typename TL, typename Comp>
struct Contains
{
	enum
	{
		value = std::is_same<typename TL::First, Comp>::value || Contains<typename TL::Rest, Comp>::value
	};
};

template<typename Comp>
struct Contains<NullType, Comp>
{
	enum{ value = false };
};

template<typename T>
class Log
{
public:

	BOOST_STATIC_ASSERT_MSG((Contains<my_typelist, T>::value), "s");

	void operator()(T log_object)
	{
		std::cout << "Log: " << log_object.message << std::endl;
	}
private:
};

template<>
struct Log<LogTraffic>
{
	void operator()(LogTraffic log_traffic)
	{		
		std::vector<std::string> vec;
		
		vec.push_back("Traffic information: \n");
		vec.push_back("Direction: ");
		vec.push_back(direction_names[log_traffic.direction]);
		vec.push_back("\nNumber of vehicles: ");
		vec.push_back(std::to_string(log_traffic.numberOfVehicles));
		vec.push_back("\nNumber of emergency vehicles: ");
		vec.push_back(std::to_string(log_traffic.numberOfEmergencies) + "\n");
		
		//std::stringstream message;

		/*message << "Traffic information: " << std::endl << "Direction: " << direction_names[log_traffic.direction] << std::endl
			<< "Number of vehicles: " << log_traffic.numberOfVehicles << std::endl
			<< "Number of emergency vehicles: " << log_traffic.numberOfEmergencies << std::endl;
		*/

		WriteToFile("output.txt", vec);
	}
};

struct TrafficInfo
{
	void TrafficMilestone(LogTraffic log_traffic)
	{
		totalVehicle += log_traffic.numberOfVehicles;

		if (static_cast<int>(totalVehicle / 100) > reachedMilestone)
		{
			reachedMilestone = totalVehicle / 100;
			std::cout << "You are vehicle number: " << totalVehicle << " and we are at milestone: " << reachedMilestone << std::endl;
		}

		else
		{
			std::cout << "Still at " << reachedMilestone << " milestone" << std::endl;
		}
	}
private:
	int totalVehicle = 0;
	int reachedMilestone = 0;
};

boost::signals2::signal<void(LogInfo)> sigInfo;
boost::signals2::signal<void(LogError)> sigError;
boost::signals2::signal<void(LogTraffic)> sigTraffic;

struct LogMessage : public boost::static_visitor<>
{
	void operator()(LogInfo info)
	{
		sigInfo(info);
	}

	void operator()(LogError error)
	{
		sigError(error);
	}

	void operator()(LogTraffic traffic)
	{
		sigTraffic(traffic);
	}
};


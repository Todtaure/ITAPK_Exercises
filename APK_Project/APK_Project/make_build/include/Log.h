#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <boost/variant.hpp>
#include <boost/signals2/signal.hpp>
#include <fstream>
#include <ctime>
#include "../include/SCUtils.h"
#include <mutex>
#include <vector>

/*Logging type classes*/

struct Log
{
	std::string message;
};

struct LogInfo : public Log
{
};

struct LogError : public Log
{
	std::string error_code_;
};

static const char* direction_names[] = { "NORTH", "SOUTH", "WEST", "EAST" };

struct LogTraffic
{
	SCUtils::Direction direction;
	int numberOfVehicles;
	int numberOfEmergencies;
};

/*IO methods*/

inline void WriteToFile(std::string path, std::vector<std::string> vec)
{
	std::mutex fileLock;
	try
	{
		fileLock.lock();
		std::fstream fs(path, std::fstream::out | std::fstream::app);

		/*Set exceptions error mask. failbit: logical i/o error. badbit: read/write error*/
		fs.exceptions(std::ostream::failbit | std::ostream::badbit);

		std::ostream_iterator<std::string> output(fs);
		std::copy(vec.begin(), vec.end(), output);
		
		/*More simple implementation below
		std::ostream_iterator<std::string> output(fs);
		fs << ss.rdbuf();
		*/

		fs.close();
		fileLock.unlock();
	}
	catch (std::ostream::failure e)
	{
		std::cout << "Couldn't write log to file." << std::endl;
		fileLock.unlock();
	}
}

/*Logger classes*/

/*Logger base template restrictor*/
typedef SCUtils::Typelist<Log, SCUtils::Typelist<LogInfo, SCUtils::Typelist<LogError, SCUtils::NullType>  > > my_typelist;

template<typename T>
class Logger
{
public:

	BOOST_STATIC_ASSERT_MSG((SCUtils::Contains<my_typelist, T>::value), "Type is not valid for base template Logger<T>");

	void operator()(T log_object)
	{
		std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		std::cout << "Logger " << ctime(&currentTime) << ": " << log_object.message << std::endl;
	}
};

template<>
struct Logger<LogTraffic>
{
	void operator()(LogTraffic log_traffic)
	{		
		log(log_traffic);
	}

	void operator()(std::list<LogTraffic> log_traffic)
	{
		/*Using foreach and std::bind to call log*/
		std::for_each(log_traffic.begin(), log_traffic.end(), std::bind(&Logger::log, this, std::placeholders::_1));
	}

private:
	void log(LogTraffic log_traffic)
	{
		std::vector<std::string> vec;

		vec.push_back("Traffic information: \n");
		vec.push_back("Direction: ");
		vec.push_back(direction_names[log_traffic.direction]);
		vec.push_back("\nNumber of vehicles: ");
		vec.push_back(std::to_string(log_traffic.numberOfVehicles));
		vec.push_back("\nNumber of emergency vehicles: ");
		vec.push_back(std::to_string(log_traffic.numberOfEmergencies) + "\n");

		/*More simple implentation below*/
		//std::stringstream message;
		/*message << "Traffic information: " << std::endl << "Direction: " << direction_names[log_traffic.direction] << std::endl
		<< "Number of vehicles: " << log_traffic.numberOfVehicles << std::endl
		<< "Number of emergency vehicles: " << log_traffic.numberOfEmergencies << std::endl;
		*/

		WriteToFile("output.txt", vec);
	}
};


/*Signal2 */
static boost::signals2::signal<void(Log)> sigLog;
static boost::signals2::signal<void(LogTraffic)> sigTraffic;
static boost::signals2::signal<void(std::list<LogTraffic>)> sigTotalTraffic; //TODO: Merge med sigTraffic og brug variant?

struct LogMessage : public boost::static_visitor<>
{
	void operator()(Log log) const
	{
		sigLog(log);
	}

	void operator()(LogTraffic traffic) const
	{
		sigTraffic(traffic);
	}

	void operator()(std::list<LogTraffic> traffic) const
	{
		sigTotalTraffic(traffic);
	}
};

struct TrafficInfo
{
	void operator()(std::list<LogTraffic> list)
	{
		for(auto &item : list)
		{
			TrafficMilestone(item);
		}
	}

	void TrafficMilestone(LogTraffic log_traffic)
	{
		totalVehicle += log_traffic.numberOfVehicles;

		if (static_cast<int>(totalVehicle / 1000) > reachedMilestone)
		{
			reachedMilestone = totalVehicle / 1000;
			std::cout << "You are vehicle number: " << totalVehicle << " and we are at milestone: " << reachedMilestone << std::endl;
		}

	}
private:
	int totalVehicle = 0;
	int reachedMilestone = 0;
};

#endif
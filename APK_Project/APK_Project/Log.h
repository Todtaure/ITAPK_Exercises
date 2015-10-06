#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <boost/variant.hpp>
#include <boost/signals2/signal.hpp>
#include <fstream>
#include <ctime> //Very much NOT unused!
#include "SCUtils.h"
#include <mutex>

/*Logging type classes*/

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
		std::cout << "Couldn't write to file" << std::endl;
		fileLock.unlock();
	}
}

/*Log classes*/

/*Log base template restrictor*/
typedef SCUtils::Typelist<LogInfo, SCUtils::Typelist<LogError, SCUtils::NullType> > my_typelist;

template<typename T>
class Log
{
public:

	BOOST_STATIC_ASSERT_MSG((SCUtils::Contains<my_typelist, T>::value), "Type is not valid for base template Log<T>");

	void operator()(T log_object)
	{
		std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		std::cout << "Log " << ctime(&currentTime) << ": " << log_object.message << std::endl;
	}
private:
};

template<>
struct Log<LogTraffic>
{
	void operator()(LogTraffic log_traffic)
	{		
		log(log_traffic);
	}

	void operator()(std::list<LogTraffic> log_traffic)
	{
		/*Using foreach and std::bind to call log*/
		std::for_each(log_traffic.begin(), log_traffic.end(), std::bind(&Log::log, this, std::placeholders::_1));
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


/*Signal2 */
static boost::signals2::signal<void(LogInfo)> sigInfo;
static boost::signals2::signal<void(LogError)> sigError;
static boost::signals2::signal<void(LogTraffic)> sigTraffic;
static boost::signals2::signal<void(std::list<LogTraffic>)> sigTotalTraffic;

static boost::variant<LogTraffic, LogInfo, LogError, std::list<LogTraffic> > LogVariant;

struct LogMessage : public boost::static_visitor<>
{
	void operator()(LogInfo info) const
	{
		sigInfo(info);
	}

	void operator()(LogError error) const
	{
		sigError(error);
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


#endif
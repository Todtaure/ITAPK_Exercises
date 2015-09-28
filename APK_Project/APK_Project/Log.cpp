#include <iostream>
#include <boost/variant.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/type_traits/is_base_of.hpp>


struct LogInfo
{
	std::string message;
};

struct LogError
{
	std::string message;
};

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


template<typename T, typename D>
struct Typelist
{
	typedef T first;
	typedef D rest;
};

typedef Typelist<LogInfo, LogError> typelist;


template<typename T>
class Log
{
public:
	BOOST_STATIC_ASSERT((boost::is_base_of<typelist, T>::value));

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
		std::cout << "Traffic information: " << std::endl << "Direction: " << log_traffic.direction << std::endl 
			<< "Number of vehicles: " << log_traffic.numberOfVehicles << std::endl 
			<< "Number of emergency vehicles: " << log_traffic.numberOfEmergencies << std::endl;
	}
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




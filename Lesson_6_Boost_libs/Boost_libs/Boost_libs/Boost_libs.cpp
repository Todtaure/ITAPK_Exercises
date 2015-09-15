// Boost_libs.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "boost/signals2/signal.hpp"
#include <iostream>

void Freefunct1(const std::string &sensorName, double sensorValue)
{
	std::cout << "Free function" << std::endl;
	std::cout << "sensorName: " << sensorName << ", " << "sensorValue: " << sensorValue << std::endl;
}

struct FunctorFunction
{
	void operator()(const std::string &sensorName, double sensorValue)
	{
		std::cout << "Functor thinghy" << std::endl;
		std::cout << "sensorName: " << sensorName << ", " << "sensorValue: " << sensorValue << std::endl;
	}
};

class ConnecterClass : public boost::signals2::trackable
{
public:
	void Something(const std::string &sensorName, double sensorValue)
	{
		std::cout << "connector class" << std::endl;
		std::cout << "sensorName: " << sensorName << ", " << "sensorValue: " << sensorValue << std::endl;
	}		
};

double AddFunc(double a, double b, double c)
{
	return a + b + c;
}

double MultiplyFunc(double a, double b, double c)
{
	return a * b * c;
}

double SubtractionFunc(double a, double b, double c)
{
	return a - b - c;
}

double AddSubFunc(double a, double b, double c)
{
	return a + b - c;
}

template<typename T>
struct Median
{
	typedef T result_type;

	template<typename Iter>
	T operator()(Iter first, Iter last) const
	{
		if (first == last)
		{
			return T();
		}

		std::vector<T> vec(4);
		std::copy(first, last, vec.begin());

		std::sort(vec.begin(), vec.end());
		
		if (vec.size() % 2 != 0)
		{
			return vec[vec.size() / 2];
		}
		else
		{
			return (vec[vec.size() / 2] + vec[(vec.size() / 2) - 1]) / 2;
		}
	} 
};

int _tmain(int argc, _TCHAR* argv[])
{
	// Exercise 1
	boost::signals2::signal<void(const std::string &, double)> sig;

	std::string tmp = "tmp";
	
	sig.connect(&Freefunct1);
	sig.connect(FunctorFunction());

	// Exercise 2
	{
		ConnecterClass test;

		sig.connect(boost::bind(&ConnecterClass::Something, boost::ref(test), _1, _2));

		sig(tmp, 42.9);		
	}	
	
	sig(tmp, 4.1);

	sig.disconnect_all_slots();
	
	// Exercise 3

	boost::signals2::signal<double(double, double, double), Median<double> > sig_exercise3;
	sig_exercise3.connect(&AddFunc);
	sig_exercise3.connect(&AddSubFunc);
	sig_exercise3.connect(&MultiplyFunc);
	sig_exercise3.connect(&SubtractionFunc);

	std::cout << "Median: " << sig_exercise3(2, 5, 6) << std::endl;

	return 0;
}


/* Exercise 2 
What is needed to make an object trackable?
To make an object trackable you need to inherit from boost::signals2::trackable

Is it thread-safe?
No, if the object is destroyed in another thread than the one you are signaling in it could break. 

*/

/* Exercise 3



*/



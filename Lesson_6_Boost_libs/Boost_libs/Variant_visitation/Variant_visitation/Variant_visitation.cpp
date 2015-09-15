#include <iostream>
#include "boost/variant/variant.hpp"
#include "boost/variant.hpp"

struct RandomObj
{
	std::string a = "abc";
	double b = 42;

	friend std::ostream &operator<<(std::ostream &os, const RandomObj &random_obj);	
};

struct RandomObj2
{
	long blub = 2400;
	char blob = 'x';
};

std::ostream &operator<<(std::ostream &os, const RandomObj &random_obj)
{
	os << random_obj.a << " || " << random_obj.b;	
	return os;
}

struct MyVisitor : public boost::static_visitor<>
{
	void operator()(int a) const
	{
		std::cout << "overload of int: " << a << std::endl;
	}

	void operator()(std::string b) const
	{
		std::cout << "overload of string: " << b << std::endl;
	}

	void operator()(RandomObj2 random_obj2) const
	{
		std::cout << "overload of long in RO2: " << random_obj2.blub << std::endl;
		std::cout << "overload of char in RO2: " << random_obj2.blob << std::endl;
	}
};


int main()
{
	RandomObj random_obj;
	boost::variant<int, std::string, RandomObj> variantObj;

	variantObj = random_obj;
	std::cout << variantObj << std::endl;

	// Exercise 2

	RandomObj2 random_obj2;
	boost::variant<int, std::string, RandomObj2> variantObj2;
	variantObj2 = random_obj2;
	boost::apply_visitor(MyVisitor(), variantObj2);
	
	getchar();
	return 0;
}

/*
	Exercise 2
	For at sørge for at alle typerne bliver besøgt sender man en visitor med, hvilken sørger for at besøge alle typerne. 
	Static visitor håndtere alle typerne i varianten. 
	Visitoren håndtere alle situationer for varianten. 
*/

/*
	Exercise 3
	Discussion
	Eks. logning af events i forbindelse med vores projekt til APK, med tilhørende visitor der håndterer de enkelte events.
*/
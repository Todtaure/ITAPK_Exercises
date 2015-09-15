#include <iostream>
#include "boost\test\minimal.hpp"

/*Exercsise 2*/

template<int bin>
struct IsBinary
{
};

template<>
struct IsBinary<0>
{
	enum { value = 0 };
};

template<>
struct IsBinary<1>
{
	enum { value = 1 };
};

template <size_t N>
struct Binary
{
	static const size_t value = Binary <N / 10 >::value << 2 | IsBinary<N % 10>::value;
};

template <>
struct Binary <0>
{
	static const size_t value = 0;
};



int test_main(int argc, char* argv[])
{
	std::cout << "Binary <1011 >:: value = " << Binary <1011 >::value << std::endl;
	/* Should generate a compiler error , but does not! */


	getchar();
	return NO_ERROR;
};

/*
Exercise 1:
	Static polymorphism: Templates, instantiated compile time.
	Dynamic polymorphism: Inheritance, base classes and implementations checked runtime.

	STL container make use of static poly. since they use template bases.

	Static vs. dynamic in c++:
	Static costs more on compile time, but errors are found while compiling and not on runtime, making correction of
	errors easier (cheaper) and it is faster on run time.

	Dynamic poly. is often more natural to use (inheritance) since objects are coupled the way we expect them.



*/
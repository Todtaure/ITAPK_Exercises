#include <iostream>
#include "MyArray.h"
#include <boost/test/minimal.hpp>

int test_main(int argc, char* argv[])
{
	std::cout << "Beginning test cases." << std::endl;

	size_t const SIZE = 5;
	MyArray<int, SIZE> arr;
	MyArray<double, SIZE> arr_ex13;

	BOOST_REQUIRE(arr.size() == SIZE);

	int filler = 15;

	arr.fill(filler);

	for (int i = 0; i < SIZE; i++)
	{
		BOOST_CHECK(arr[i] == filler);
	}

	arr[0] = 1;
	arr[SIZE - 1] = 2;

	BOOST_CHECK(*arr.begin() == 1);
	BOOST_CHECK(arr.end() == arr.begin() + SIZE);

	int const searchVal = 3;
	arr[3] = searchVal;
	arr_ex13[3] = searchVal;

	BOOST_CHECK(myFind(arr.begin(), arr.end(), searchVal) == &arr[3]);

	BOOST_CHECK(myFind<double>(arr_ex13.begin(), arr_ex13.end(), searchVal) == &arr_ex13[3]);

	std::cout << "Test cases completed." << std::endl;

	getchar();
	return 0;
}

/*
Exercise 1.3
	BOOST_CHECK(myFind<double>(arr_ex13.begin(), arr_ex13.end(), searchVal) == &arr_ex13[3]);
	This has to be added because otherwise the compiler can´t decide whether T should be substitued by
	int (the search parameter) or double (end and begin pointers)

*/
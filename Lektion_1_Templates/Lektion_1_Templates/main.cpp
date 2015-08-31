#include <iostream>
#include "MyArray.h"
#include <boost/test/minimal.hpp>
#include <vector>

int test_main(int argc, char* argv[])
{
	std::cout << "Beginning test cases." << std::endl;

	size_t const SIZE = 5;
	MyArray<int, SIZE> arr;
	MyArray<double, SIZE> arr_ex13;

	BOOST_REQUIRE(arr.size() == SIZE);

	int filler = 15;

	arr.fill(filler);

	//Exercise 1
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

	//Exercise 1.2
	BOOST_CHECK(myFind(arr.begin(), arr.end(), searchVal) == &arr[3]);

	//Test exercise 1.3
	BOOST_CHECK(myFind(arr_ex13.begin(), arr_ex13.end(), searchVal) == &arr_ex13[3]);

	for (int i = 0; i < SIZE;i++)
	{
		arr[i] = i;
	}
	for (int i = SIZE; i <= 0; i--)
	{
		arr_ex13[i] = i + 0.7;
	}

	MyArray<int, SIZE> arr_ex14(arr_ex13);

	for (int i = 0; i < SIZE; i++)
	{
		BOOST_CHECK(arr_ex13[i] == arr_ex14[i]);
	}

	//Test exercise 1.4
	arr_ex13 = arr;

	for (int i = 0; i < SIZE; i++)
	{
		BOOST_CHECK(arr_ex13[i] == arr[i]);
	}

	//Exercise 2
	MyArray<std::string*, SIZE> arr_ex2;
	std::string *somestring = new std::string("hello");

	arr_ex2.fill(somestring);

	BOOST_CHECK(*arr_ex2[0] == *somestring);
	BOOST_CHECK(*arr_ex2.begin() == somestring);
	BOOST_CHECK(*myFind(arr_ex2.begin(), arr_ex2.end(), "hello") == somestring);
	

	//Exercise 3
	std::vector<int> vec = { 1,1,1,1,1,1 };

	BOOST_CHECK(myAccumulation(vec) == 6);
	BOOST_CHECK(myAccumulation(arr) == 10);

	std::cout << "Test cases completed." << std::endl;

	getchar();
	return 0;
}

/*
Exercise 1.3
	BOOST_CHECK(myFind<double>(arr_ex13.begin(), arr_ex13.end(), searchVal) == &arr_ex13[3]);
	This has to be added because otherwise the compiler can´t decide whether T should be substitued by
	int (the search parameter) or double (end and begin pointers)¨.

Exercise 2.3
	Dunno

Exercise 3
	1) U::value_type represents the type that the chosen container contains, eg. vector<int> it would be int.
	   
	2) This line calls the value_type() method which every container iterator has and it returns the type
	of the object that is returned when the iterator is dereferenced.

	3) It is requiered that the container or object used implements the iterator interface.
	   
*/
#include <iostream>
#include "boost\test\minimal.hpp"
#include "MetaProgramming.h"


int test_main(int argc, char* argv[])
{
	/*Exercise 2*/
	std::cout << "Binary <1011 >:: value = " << Binary<1011 >::value << std::endl;
	/* Should generate a compiler error , but does not! */

	/*Exercise 3.1*/
	mixedTypes::Rest::Rest::First doubleValue = 2.0;
	myTypes::First integerValue = 2;
	/*
	Hvis noget kun skal køre med bestemte tyåer kan static assert anvendes til at set om argumentet
	er i den Typelist som specificerer parametre.
	Ifm. Boost test kan en typelist bruges til at automatisere tests ift. oprettelse af typer.
	*/

	/*Exercise3.2.1*/
	/* Must be true */
	std::cout << "IsSame <int , int >:: value=" << IsSame<int, int >::value << std::endl;

	/* Must be false */
	std::cout << "IsSame <int , char >:: value=" << IsSame<int, char >::value << std::endl;

	/*Exercise 3.2.2*/
	/* Must be true */
	std::cout << "Contains <TL , int >:: value=" << Contains<TL, int>::value << std::endl;

	/* Must be false */
	std::cout << "Contains <TL , std::string >::value=" << Contains<TL, std::string>::value << std::endl;

	/*Exercise 3.2.3*/
	/* Must be true */
	std::cout << "IsSame <typename AtIndex <TL , 2>::type , int >::value " << 
		IsSame<typename AtIndex<TL_ex323, 2>::type, int>::value << std::endl;

	/* Must be false */
	std::cout << "IsSame <typename AtIndex <TL , 2>::type , char >::value " << 
		IsSame<typename AtIndex<TL_ex323, 2>::type, char>::value << std::endl;


	/*Exercise 3.2.4*/
	std::cout << "Type in TypeList: ";
	PrintIT<TL_ex323>();



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
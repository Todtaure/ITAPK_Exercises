#include "SharedPtr.h"
#include <string>

using namespace std;


int main()
{
	cout << "Starting ...." << endl;
	int number = 42;
	int someInt = 10;

	SharedPtr<int> myPtr(&number);
	SharedPtr<int> secPtr(myPtr);
	SharedPtr<int> someIntPtr(&someInt);

	cout << "First Pointer : " << *myPtr << endl;
	cout << "Second Pointer: " << *secPtr << endl;

	cout << "Pointers the same? " << (myPtr == secPtr ? "Yes" : "No") << endl;
	cout << "Pointers the same? " << (myPtr == someIntPtr ? "Yes" : "No") << endl;
	
	cout << "Count is: " << secPtr.count() << " for " << *secPtr << endl;
	cout << "Count is: " << someIntPtr.count() << " for " << *someIntPtr << endl;
		
	cout << "First Pointer: " << *myPtr << endl;
	myPtr.~SharedPtr();
	cout << "Count is: " << secPtr.count() << " for " << *secPtr << endl;

	getchar();
	return 0;
}

//Exercise 2
/*
2.1
There is no need for explicit, since we do not have an overload of the + operator
(thus not allowing operations like "myPtr + 10") 
or the assignment operator like "myPtr = 10".

int number = 42;
SharedPtr<int> myPtr(&number);
myPtr = 10; //No
myPtr = myPtr + 10 //No


2.2
The assignment operator has been overloaded in accordance with rule of three, as well as the pointer operator 
and the class-member access operator in accordance with the Smart Pointer idiom.

Other useful overloads could include the plus(+) operator for adding operations, creating a new element.



*/
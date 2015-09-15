#include "SharedPtr.h"

using namespace std;

int main()
{
	cout << "Starting ...." << endl;
	int number = 42;
	int someInt = 10;
	{
		CustomPtr::SharedPtr<int> myPtr(&number);
		CustomPtr::SharedPtr<int> someIntPtr(&someInt);
		{		
			CustomPtr::SharedPtr<int> secPtr(myPtr);

			cout << "First Pointer : " << *myPtr << endl;
			cout << "Second Pointer: " << *secPtr << endl;

			cout << "Pointers the same? " << (myPtr == secPtr ? "Yes" : "No") << endl;
			cout << "Pointers the same? " << (myPtr == someIntPtr ? "Yes" : "No") << endl;

			cout << "Count is: " << secPtr.count() << " for " << *secPtr << endl;
			cout << "Count is: " << someIntPtr.count() << " for " << *someIntPtr << endl;
		}
		cout << "First Pointer: " << *myPtr << endl;
		cout << "Count is: " << myPtr.count() << " for " << *myPtr << endl;
		
	}

	int someNumber = 50;
	CustomPtr::SharedPtr<int> customDeletePtr(&someNumber, CustomPtr::HelperFunctor<int>());

	cout << "Ptr with custom delete: " << *customDeletePtr << endl;

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
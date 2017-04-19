#ifndef TEST_CPP
#define TEST_CPP	

#include <iostream>

#include "consoleWindowManager.h"

void test() {
	int value = 5;
	int * value2 = &value;

	ConsoleWindowManager cwm;
	cout << endl;

	cout << "Value: " << value << endl;
	cout << "Value pointer: " << &value << endl;
	cout << "Value 2 pointer: " << value2 << endl;
	cout << "Value 2 value: " << *value2 << endl;
	cout << endl;

	*value2 = 10;

	cout << "Changed value: " << value << endl;
	cout << "Changed value pointer: " << &value << endl;
	cout << "Changed value 2 pointer: " << value2 << endl;
	cout << "Changed value 2 value: " << *value2 << endl;


	/*for (int i = 1; i != 1000000; i++) {
	ConsoleWindowManager* cwm2 = new ConsoleWindowManager();
	cwm2->consoleTest();
	cout << i << "db" << endl;
	delete cwm2;
	}*/
}

#endif
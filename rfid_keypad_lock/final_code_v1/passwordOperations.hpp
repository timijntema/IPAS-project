/*
* File:   passwordOperations.hpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 17:13
*/

#ifndef PASSWORDOPERATIONS_HPP
#define PASSWORDOPERATIONS_HPP

#include "matrixKeypad.hpp"
#include "hwlib.hpp"

class passwordOperations{
private:
	matrixKeypad & keypad;
	int lenRootPWD;
	char * rootPWD;
public:
	passwordOperations(matrixKeypad & keypad, char * rootPWD, int lenRootPWD);
	bool getPassword(char * chararray, int lenCharArray);
	bool setPassword(char * clientPWD, int lenCharArray);
};

#endif // PASSWORDOPERATIONS_HPP

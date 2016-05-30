/*
* File:   passwordOperations.hpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 17:13
*/

#ifndef PASSWORDOPERATIONS_HPP
#define PASSWORDOPERATIONS_HPP

#include "matrixKeypad.hpp"
#include <string>

class passwordOperations{
private:
	matrixKeypad & keypad;
	std::string & rootPWD;
public:
	passwordOperations(matrixKeypad & keypad, std::string & rootPWD);
	bool getPassword(std::string & clientPWD);
	bool setPassword(std::string & clientPWD);
};

#endif // PASSWORDOPERATIONS_HPP

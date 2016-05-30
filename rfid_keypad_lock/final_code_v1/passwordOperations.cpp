/*
* File:   passwordOperations.cpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 17:13
*/

#include "passwordOperations.hpp"

passwordOperations::passwordOperations(matrixKeypad & keypad, std::string & rootPWD):
		keypad(keypad),
		rootPWD(rootPWD)
{}

bool passwordOperations::getPassword(std::string & clientPWD){
	std::string tmpPwd;
	keypad.getString(tmpPwd);
	
	if (tmpPwd == clientPWD){
		hwlib::cout << "true";
		return true;
	}
	else{
		hwlib::cout << "false";
		return false;
	}
}
bool passwordOperations::setPassword(std::string & clientPWD){
	if (getPassword(rootPWD)){
		keypad.getString(clientPWD);
		return true;
	}
	else {
		return false;
	}
}
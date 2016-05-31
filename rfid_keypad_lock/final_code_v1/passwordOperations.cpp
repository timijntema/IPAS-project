/*
* File:   passwordOperations.cpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 17:13
*/

#include "passwordOperations.hpp"

passwordOperations::passwordOperations(matrixKeypad & keypad, char * rootPWDf, int lenRootPWD):
		keypad(keypad),
		lenRootPWD(lenRootPWD),
		rootPWD(rootPWDf)
{
	//rootPWD = rootPWDf;
}

bool passwordOperations::getPassword(char * clientPWD, int lenCharArrayClient){
	int lenCharArrayTmp = lenCharArrayClient+10;
	char tempArray[lenCharArrayTmp];
	lenCharArrayTmp = keypad.getString(tempArray, lenCharArrayTmp);
	
	if (lenCharArrayTmp != lenCharArrayClient){
		hwlib::cout <<  lenCharArrayTmp << ' ' << lenCharArrayClient << "\nsizes unequal\n";
		return false;
	}
	
	for(int i = 0; i < lenCharArrayTmp; i++) {
		//hwlib::cout << tempArray[i] << '\n';
		if (tempArray[i] != clientPWD[i]){
			hwlib::cout << "false\n";
			return false;
		}
	}
	hwlib::cout << "true\n";
	return true;
}
bool passwordOperations::setPassword(char * clientPWD, int lenCharArray){
	if (getPassword(rootPWD, lenRootPWD)){
		hwlib::cout << "type the client PASSWORD\n";
		hwlib::wait_ms(1000);
		keypad.getString(clientPWD, lenCharArray);
		return true;
	}
	return false;
}
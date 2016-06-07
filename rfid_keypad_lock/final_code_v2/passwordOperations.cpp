/*
* File:   passwordOperations.cpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 17:13
*/

#include "passwordOperations.hpp"

passwordOperations::passwordOperations(matrixKeypad & keypad, char * rootPWDf, int lenRootPWD, hwlib::target::pin_out & ledGreen, hwlib::target::pin_out & ledRed):
		keypad(keypad),
		lenRootPWD(lenRootPWD),
		rootPWD(rootPWDf),
		ledGreen(ledGreen),
		ledRed(ledRed)
{}

bool passwordOperations::getPassword(char * clientPWD, int lenCharArrayClient){
	int lenCharArrayTmp = lenCharArrayClient+10;
	char tempArray[lenCharArrayTmp];
	int lenAfter = 0;
	
	for(int i = 0; i < 3; i++){//ask for a password 3 times maximum
		lenAfter = keypad.getString(tempArray, lenCharArrayTmp);
		
		//check if the returned length is correct
		if (lenAfter != lenCharArrayClient){
			hwlib::cout  << "Try again\n";
			for(int i = 0; i < 3; i++){
				ledRed.set(0);
				hwlib::wait_ms(50);
				ledRed.set(1);
				hwlib::wait_ms(50);
			}
			continue;
		}
		
		//check all characters in the array
		for(int i = 0; i < lenAfter; i++) {
			if (tempArray[i] != clientPWD[i]){
				hwlib::cout << "Try again\n";
				for(int i = 0; i < 3; i++){
					ledRed.set(0);
					hwlib::wait_ms(50);
					ledRed.set(1);
					hwlib::wait_ms(50);
				}
				break;
			}
			//if the last character is also correct return true
			else if (i == (lenAfter-1) && tempArray[i] == clientPWD[i]){
				return true;
			}
		}
	}
	return false;
}

bool passwordOperations::setPassword(char * clientPWD, int lenCharArray){
	if (getPassword(rootPWD, lenRootPWD)){//check for root access
		hwlib::cout << "type the client PASSWORD\n";
		ledRed.set(0);
		for(int i = 0; i<10; i++){
			ledGreen.set(1);
			hwlib::wait_ms(50);
			ledGreen.set(0);
			hwlib::wait_ms(50);
		}
		ledRed.set(1);
		keypad.getString(clientPWD, lenCharArray);
		return true;
	}
	return false;
}
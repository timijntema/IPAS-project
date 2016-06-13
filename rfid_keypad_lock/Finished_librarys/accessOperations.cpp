/*
* File:   accessOperations.cpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 17:13
*/

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#include "accessOperations.hpp"

accessOperations::accessOperations(matrixKeypad & keypad, char * rootPWD, int lenRootPWD, hwlib::pin_out & ledGreen, hwlib::pin_out & ledRed):
		keypad(keypad),
		lenRootPWD(lenRootPWD),
		rootPWD(rootPWD),
		ledGreen(ledGreen),
		ledRed(ledRed)
{}

bool accessOperations::getPassword(char * clientPWD, int lenCharArrayClient){
	int lenCharArrayTmp = lenCharArrayClient+10;
	char tempArray[lenCharArrayTmp];
	int lenAfter = 0;
	
	for(int i = 0; i < 3; i++){//Ask for a password 3 times maximum
		lenAfter = keypad.getString(tempArray, lenCharArrayTmp);
		
		//Check if the returned length is correct
		if (lenAfter != lenCharArrayClient){
			hwlib::cout  << "\nTry again\n";
			for(int i = 0; i < 3; i++){
				ledRed.set(0);
				hwlib::wait_ms(50);
				ledRed.set(1);
				hwlib::wait_ms(50);
			}
			continue;
		}
		
		//Check all characters in the returned array
		for(int i = 0; i < lenAfter; i++) {
			if (tempArray[i] != clientPWD[i]){
				hwlib::cout << "\nTry again\n";
				for(int i = 0; i < 3; i++){
					ledRed.set(0);
					hwlib::wait_ms(50);
					ledRed.set(1);
					hwlib::wait_ms(50);
				}
				break;
			}
			//If the last character is also correct return true
			else if (i == (lenAfter-1) && tempArray[i] == clientPWD[i]){
				return true;
			}
		}
	}
	return false;
}

bool accessOperations::setPassword(char * clientPWD, const int & lenCharArray, int *currentArrayLocation){//if it dusnt work do the currentlocation++ in main and if fail do --
	if (getPassword(rootPWD, lenRootPWD)){//Check for root access
		ledRed.set(0);
		for(int i = 0; i<10; i++){
			ledGreen.set(1);
			hwlib::wait_ms(50);
			ledGreen.set(0);
			hwlib::wait_ms(50);
		}
		ledRed.set(1);
		
		//Make sure the current array location is lower than 10 to avoid going out of array.
		if(*currentArrayLocation < 10){
			hwlib::cout << "\ntype the client PASSWORD\n";
			keypad.getString(clientPWD, lenCharArray);
			return true;
		}
	}
	return false;
}

bool accessOperations::checkSingleID(byte * ID, int lenID, byte * checkID){
	//Compare the 2 arrays and return false if the are not equeal at any point
	for(int i = 0; i < lenID; i++){
		if(ID[i] != checkID[i]){
			return false;
		}
		
		//Return true if the end of the array has been reached and the array positions are still equal
		else if(i == (lenID-1)){
			return true;
		}
	}
	return false;
}

bool accessOperations::checkMultipleID(byte * ID, int lenID, int lenAccesIDs, byte (*accessIDs)[5], int * arrayLocation){
	//Loop through the multidimensional array and keep on sending individual ID's to the checkSingleID function
	for(int j = 0; j < lenAccesIDs; j++){
		//Return true and the array location if it finds a match
		if(checkSingleID(ID, lenID, accessIDs[j])){
			*arrayLocation = j;
			return true;
		}
	}
	return false;
}
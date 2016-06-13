/*
* File:   accessOperations.cpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 17:13
*/

#include "accessOperations.hpp"

accessOperations::accessOperations(matrixKeypad & keypad, char * rootPWDf, int lenRootPWD, hwlib::pin_out & ledGreen, hwlib::pin_out & ledRed):
		keypad(keypad),
		lenRootPWD(lenRootPWD),
		rootPWD(rootPWDf),
		ledGreen(ledGreen),
		ledRed(ledRed)
{}

bool accessOperations::getPassword(char * clientPWD, int lenCharArrayClient){
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

bool accessOperations::setPassword(char * clientPWD, const int & lenCharArray, int *currentArrayLocation){//if it dusnt work do the currentlocation++ in main and if fail do --
	if (getPassword(rootPWD, lenRootPWD)){//check for root access
		hwlib::cout << "\ntype the client PASSWORD\n";
		ledRed.set(0);
		for(int i = 0; i<10; i++){
			ledGreen.set(1);
			hwlib::wait_ms(50);
			ledGreen.set(0);
			hwlib::wait_ms(50);
		}
		ledRed.set(1);
		if(*currentArrayLocation < 10){
			keypad.getString(clientPWD, lenCharArray);
			return true;
		}
	}
	return false;
}

bool accessOperations::checkSingleID(byte * ID, int lenID, byte * checkID){
	for(int i = 0; i < lenID; i++){
		if(ID[i] != checkID[i]){
			return false;
		}
		else if(i == (lenID-1)){
			return true;
		}
	}
	return false;
}

bool accessOperations::checkMultipleID(byte * ID, int lenID, int lenAccesIDs, byte (*accessIDs)[5], int * arrayLocation){
	for(int j = 0; j < lenAccesIDs; j++){
		if(checkSingleID(ID, lenID, accessIDs[j])){
			*arrayLocation = j;
			return true;
		}
	}
	return false;
}
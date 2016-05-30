/*
* File:   main.cpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 17:09
*/

#include "hwlib.hpp"
#include "matrixKeypad.hpp"
#include "passwordOperations.hpp"
#include <string>


int main(){
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	std::string rootPWD = "D97A3";
	std::string userPWD;
	
	char tempChar;
	
	//columns
	auto keypad0 = hwlib::target::pin_in_out(hwlib::target::pins::d23);
	auto keypad1 = hwlib::target::pin_in_out(hwlib::target::pins::d25);
	auto keypad2 = hwlib::target::pin_in_out(hwlib::target::pins::d27);
	auto keypad3 = hwlib::target::pin_in_out(hwlib::target::pins::d29);
	
	//rows
	auto keypad4 = hwlib::target::pin_in_out(hwlib::target::pins::d31);
	auto keypad5 = hwlib::target::pin_in_out(hwlib::target::pins::d33);
	auto keypad6 = hwlib::target::pin_in_out(hwlib::target::pins::d35);
	auto keypad7 = hwlib::target::pin_in_out(hwlib::target::pins::d30);
	
	matrixKeypad keypad(keypad0, keypad1, keypad2, keypad3, keypad4, keypad5, keypad6, keypad7, 4, 4);
	passwordOperations pswd(keypad, rootPWD);
	//servo object
	
	while(1){
		tempChar = 'k';//make sure tempChar is not something we are using in the program
		tempChar = keypad.getKey();
		if (tempChar == '#'){
			while(!(pswd.setPassword(userPWD))){
				//flash red
			}
			//flash green
		}
		else if (tempChar == '*'){
			if(pswd.getPassword(userPWD)){
				//flash green (v2 of the code)
				//turn on green (v1 of the code)
				//turn servo
				tempChar = keypad.getKey();
				//turn servo
			}
			else{
				//flash red
			}
		}
	}
	return 0;
}

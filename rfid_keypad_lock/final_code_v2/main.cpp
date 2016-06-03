/*
* File:   main.cpp
* Author: Tim IJntema
*
* Created on 2 june 2016, 11:26
*/

#include "hwlib.hpp"
#include "matrixKeypad.hpp"
#include "passwordOperations.hpp"
#include "PWM_signal.hpp"
#include "servo.hpp"


int main(){
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	char rootPWD[] = "D97A3";
	char userPWD[10];
	int lenUserPWD = sizeof(userPWD)/sizeof(userPWD[0]);
	int lenRootPWD = sizeof(rootPWD)/sizeof(rootPWD[0]);
	
	char tempChar;
	
	//columns
	auto keypad0 = hwlib::target::pin_in_out(hwlib::target::pins::d23);
	auto keypad1 = hwlib::target::pin_in_out(hwlib::target::pins::d25);
	auto keypad2 = hwlib::target::pin_in_out(hwlib::target::pins::d27);
	auto keypad3 = hwlib::target::pin_in_out(hwlib::target::pins::d29);
	
	//rows
	auto keypad4 = hwlib::target::pin_in_out(hwlib::target::pins::d22);
	auto keypad5 = hwlib::target::pin_in_out(hwlib::target::pins::d24);
	auto keypad6 = hwlib::target::pin_in_out(hwlib::target::pins::d26);
	auto keypad7 = hwlib::target::pin_in_out(hwlib::target::pins::d28);
	
	//remaining pins
	auto servoPin = hwlib::target::pin_out(hwlib::target::pins::d2);
	auto ledGreen = hwlib::target::pin_out(hwlib::target::pins::d3);
	auto ledRed = hwlib::target::pin_out(hwlib::target::pins::d4);
	auto buzzerPin = hwlib::target::pin_out(hwlib::target::pins::d5);
	
	//keypad objects
	matrixKeypad keypad(keypad0, keypad1, keypad2, keypad3, keypad4, keypad5, keypad6, keypad7, buzzerPin);
	passwordOperations pswd(keypad, rootPWD, lenRootPWD);
	
	//servo object
	servo servo1(servoPin);
	
	while(1){
		tempChar = 'k';//make sure tempChar is not something we are using in the program
		ledRed.set(1);
		tempChar = keypad.getKey();
		
		if (tempChar == '#'){//set a password with #
			hwlib::cout << "caught #\n";
			hwlib::wait_ms(500);
			hwlib::cout << "type root pwd\n";
			
			if(!(pswd.setPassword(userPWD, lenUserPWD))){
				hwlib::cout << "You need the root password to set a user password\n";
				for(int i = 0; i < 4; i++){
					ledRed.set(0);
					hwlib::wait_ms(100);
					ledRed.set(1);
					hwlib::wait_ms(100);
				}
				continue;
			}
			
			hwlib::cout << "password set\n";
			ledRed.set(0);
			for(int i = 0; i < 4; i++){
				ledGreen.set(1);
				hwlib::wait_ms(100);
				ledGreen.set(0);
				hwlib::wait_ms(100);
			}
			int i = 0;
			while(userPWD[i] != '\0'){
				i++;
			}
			hwlib::cout << "\nhier komt hij\n";
			lenUserPWD = i+1;
			ledGreen.set(0);
			hwlib::wait_ms(100);
		}
		
		
		else if (tempChar == '*'){
			hwlib::cout << "caught *\n";
			hwlib::wait_ms(500);
			hwlib::cout << "type user pwd\n";
			
			//in v2 code first check rfid and then set a bool value true or false and make sure you know what the user id is
			//then check password (see below) && previously set bool value. The password check needs to get the user of the pass.
			//if user not in the "database" flash red.
			
			if(pswd.getPassword(userPWD, lenUserPWD)){
				//flash green and then check rfid (v2 of the code)
				//turn on green and turn off red
				ledRed.set(0);
				ledGreen.set(1);
				
				hwlib::cout << "opening box\n";
				servo1.turnDegrees(180);
				tempChar = keypad.getKey();
				hwlib::cout << "closing box\n";
				servo1.turnDegrees(110);
			}
			else{
				for(int i = 0; i < 4; i++){
					ledRed.set(0);
					hwlib::wait_ms(100);
					ledRed.set(1);
					hwlib::wait_ms(100);
				}
			}
		}
		ledGreen.set(0);
		hwlib::wait_ms(100);
	}
	return 0;
}

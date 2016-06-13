/*
* File:   main.cpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 17:09
*/

#include "hwlib.hpp"
#include "matrixKeypad.hpp"
#include "accessOperations.hpp"
#include "PWM_signal.hpp"
#include "servo.hpp"


int main(){
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	char rootPWD[] = "9735";
	char userPWD[10];
	int lenUserPWD = sizeof(userPWD)/sizeof(userPWD[0]);
	int lenRootPWD = sizeof(rootPWD)/sizeof(rootPWD[0]);
	
	char tempChar;
	
	//rows
	auto keypad0 = hwlib::target::pin_in_out(hwlib::target::pins::d32);
	auto keypad1 = hwlib::target::pin_in_out(hwlib::target::pins::d22);
	auto keypad2 = hwlib::target::pin_in_out(hwlib::target::pins::d24);
	auto keypad3 = hwlib::target::pin_in_out(hwlib::target::pins::d28);
	
	//columns
	auto keypad4 = hwlib::target::pin_in_out(hwlib::target::pins::d30);
	auto keypad5 = hwlib::target::pin_in_out(hwlib::target::pins::d34);
	auto keypad6 = hwlib::target::pin_in_out(hwlib::target::pins::d26);
	auto keypad7 = hwlib::target::pin_in_out(hwlib::target::pins::d31);//created fake pin because pin dummy ir creatint a wrong column problem
	
	//remaining pins
	auto servoPin = hwlib::target::pin_out(hwlib::target::pins::d52);
	auto ledGreen = hwlib::target::pin_out(hwlib::target::pins::d3);
	auto ledRed = hwlib::target::pin_out(hwlib::target::pins::d4);
	
	//keypad objects
	matrixKeypad keypad(keypad0, keypad1, keypad2, keypad3, keypad4, keypad5, keypad6, keypad7);//buzzer support came later
	passwordOperations pswd(keypad, rootPWD, lenRootPWD);
	
	//servo objects
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
			
			if(pswd.getPassword(userPWD, lenUserPWD)){
				//flash green and then check rfid (v2 of the code)
				//turn on green and turn off red
				ledRed.set(0);
				ledGreen.set(1);
				
				hwlib::cout << "opening box\n";
				servo1.turnDegrees(155);
				tempChar = keypad.getKey();
				hwlib::cout << "closing box\n";
				servo1.turnDegrees(165);
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

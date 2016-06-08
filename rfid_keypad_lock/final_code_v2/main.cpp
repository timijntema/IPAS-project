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
	
	char rootPWD[] = "973";
	char userPWD[10];
	int lenUserPWD = sizeof(userPWD)/sizeof(userPWD[0]);
	int lenRootPWD = sizeof(rootPWD)/sizeof(rootPWD[0]);
	
	char tempChar;
	
	//rows
	auto keypad0 = hwlib::target::pin_in_out(hwlib::target::pins::d32);//34
	auto keypad1 = hwlib::target::pin_in_out(hwlib::target::pins::d22);//32
	auto keypad2 = hwlib::target::pin_in_out(hwlib::target::pins::d24);//30
	auto keypad3 = hwlib::target::pin_in_out(hwlib::target::pins::d28);//28
	
	//columns
	auto keypad4 = hwlib::target::pin_in_out(hwlib::target::pins::d30);//26
	auto keypad5 = hwlib::target::pin_in_out(hwlib::target::pins::d34);//24
	auto keypad6 = hwlib::target::pin_in_out(hwlib::target::pins::d26);//22
	auto keypad7 = hwlib::target::pin_in_out(hwlib::target::pins::d31);//created fake pin because pin dummy ir creatint a wrong column problem
	
	//remaining pins
	auto servoPin = hwlib::target::pin_out(hwlib::target::pins::d52);
	auto ledGreen = hwlib::target::pin_out(hwlib::target::pins::d3);
	auto ledRed = hwlib::target::pin_out(hwlib::target::pins::d4);
	auto buzzerPin = hwlib::target::pin_out(hwlib::target::pins::d6);
	auto closePin = hwlib::target::pin_in(hwlib::target::pins::d7);
	
	//keypad objects
	matrixKeypad keypad(keypad0, keypad1, keypad2, keypad3, keypad4, keypad5, keypad6, keypad7, buzzerPin);
	passwordOperations pswd(keypad, rootPWD, lenRootPWD, ledGreen, ledRed);
	
	//servo object
	servo servo1(servoPin);
	while(closePin.get() != 0){}
	hwlib::wait_ms(500);
	servo1.turnDegrees(145);
	
	while(1){
		tempChar = 'k';//make sure tempChar is not something we are using in the program
		ledRed.set(1);
		tempChar = keypad.getKey();
		
		if (tempChar == '#'){//set a password with #
			hwlib::cout << "caught #\n";
			ledRed.set(0);
			for(int i = 0; i<5; i++){
				ledGreen.set(1);
				hwlib::wait_ms(50);
				ledGreen.set(0);
				hwlib::wait_ms(50);
			}
			ledRed.set(1);
			hwlib::cout << "type root pwd\n";
			
			if(!(pswd.setPassword(userPWD, lenUserPWD))){
				hwlib::cout << "You need the root password to set a user password\n";
				for(int i = 0; i < 10; i++){
					ledRed.set(0);
					hwlib::wait_ms(100);
					ledRed.set(1);
					hwlib::wait_ms(100);
				}
				continue;
			}
			
			hwlib::cout << "password set\n";
			ledRed.set(0);
			ledGreen.set(1);
			hwlib::wait_ms(1000);
			ledGreen.set(0);
			ledRed.set(1);
			int i = 0;
			while(userPWD[i] != '\0'){
				i++;
			}
			lenUserPWD = i+1;;
			hwlib::wait_ms(100);
		}
		
		
		else if (tempChar == '*'){
			hwlib::cout << "caught *\n";
			ledRed.set(0);
			for(int i = 0; i<5; i++){
				ledGreen.set(1);
				hwlib::wait_ms(100);
				ledGreen.set(0);
				hwlib::wait_ms(100);
			}
			ledRed.set(1);
			hwlib::wait_ms(500);
			hwlib::cout << "type user pwd\n";
			
			//in v2 code first check rfid and then set a bool value true or false and make sure you know what the user id is
			//then check password (see below) && previously set bool value. The password check needs to get the user of the pass.
			//if user not in the "database" flash red.
			
			if(pswd.getPassword(userPWD, lenUserPWD)){
				hwlib::cout << "opening box\n";
				servo1.turnDegrees(165);
				for(int i = 0; i<15; i++){
					ledGreen.set(0);
					hwlib::wait_ms(100);
					ledGreen.set(1);
					hwlib::wait_ms(100);
				}
				
				//close sequence
				while(closePin.get() != 0){}
				hwlib::cout << "closing box\n";
				hwlib::wait_ms(500);
				servo1.turnDegrees(145);
			}
			else{
				ledGreen.set(0);
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

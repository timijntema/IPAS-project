/*
* File:   main.cpp
* Author: Tim IJntema
*
* Created on 8 june 2016, 8:21
*/

#include "hwlib.hpp"
#include "matrixKeypad.hpp"
#include "passwordOperations.hpp"
#include "PWM_signal.hpp"
#include "servo.hpp"
#include "mfrc522v2.hpp"

bool checkSingleID(byte * ID, int lenID, byte * checkID){
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

bool checkMultipleID(byte * ID, int lenID, int lenAccesIDs, byte (*accessIDs)[5], int * arrayLocation){
	for(int j = 0; j < lenAccesIDs; j++){
		if(checkSingleID(ID, lenID, accessIDs[j])){
			*arrayLocation = j;
			return true;
		}
	}
	return false;
}

int main(){
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	//keypad variables
	const int maxLen = 10;
	char PWD[maxLen][maxLen] = {"9735"};
	int PWDLen[maxLen] = {5};
	//int lenRootPWD = sizeof(PWD[0])/sizeof(PWD[0][0]);
	//char tempChar;
	
	//rfid
	const auto maxIDLen = 5;
	byte ID[maxIDLen];
	byte ID2[maxIDLen];
	byte accessIDs[maxLen][maxIDLen] = {{52,188,189,222,235}};
	int arrayLocation;
	int currentArrayLocation = 0;
	
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
	auto buzzerPin = hwlib::target::pin_out(hwlib::target::pins::d6);
	auto closePin = hwlib::target::pin_in(hwlib::target::pins::d7);
	
	//spi pins
	auto SDA = hwlib::target::pin_out(hwlib::target::pins::d8);
	auto CLK = hwlib::target::pin_out(hwlib::target::pins::d9);
	auto MOSI = hwlib::target::pin_out(hwlib::target::pins::d10);
	auto MISO = hwlib::target::pin_in(hwlib::target::pins::d11);
	auto RESET = hwlib::target::pin_out(hwlib::target::pins::d12);
	
	//keypad objects
	matrixKeypad keypad(keypad0, keypad1, keypad2, keypad3, keypad4, keypad5, keypad6, keypad7, buzzerPin);
	passwordOperations pswd(keypad, &PWD[0][0], PWDLen[0], ledGreen, ledRed);
	
	//spi and RFID
	auto spiBusSetup = hwlib::spi_bus_bit_banged_sclk_mosi_miso(CLK, MOSI, MISO);
	mfrc522v2 RFID(spiBusSetup, SDA, RESET);
	
	//servo object
	servo servo1(servoPin);
	while(closePin.get() != 0){}
	hwlib::wait_ms(500);
	servo1.turnDegrees(155);
	
	while(1){
		ledRed.set(1);
		RFID.waitForCardID(ID, maxIDLen);
		
		if(checkMultipleID(ID, maxIDLen, maxLen, accessIDs, &arrayLocation)){
			hwlib::cout << (int)arrayLocation << " Its in the id list\n";
			ledRed.set(0);
			for(int i = 0; i < 5; i++){
				ledGreen.set(0);
				hwlib::wait_ms(50);
				ledGreen.set(1);
				hwlib::wait_ms(50);
			}
			
			hwlib::cout << "Type user password";
			if(pswd.getPassword(PWD[ (arrayLocation) ], PWDLen[ (arrayLocation) ])){
				hwlib::cout << "Opening box\n";
				servo1.turnDegrees(168);
				for(int i = 0; i<15; i++){
					ledGreen.set(0);
					hwlib::wait_ms(100);
					ledGreen.set(1);
					hwlib::wait_ms(100);
				}
				
				//close sequence
				while(closePin.get() != 0){}
				hwlib::cout << "Closing box\n";
				hwlib::wait_ms(500);
				servo1.turnDegrees(155);//might need to be lower
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
			ledGreen.set(0);
			hwlib::wait_ms(100);
		}
		
		
		else{
			hwlib::cout << "Its not in the id list\n";
			for(int i = 0; i < 5; i++){
				ledRed.set(0);
				hwlib::wait_ms(50);
				ledRed.set(1);
				hwlib::wait_ms(50);
			}
			
			hwlib::cout << "Hold the root tag in front of the reader\n";
			RFID.waitForCardID(ID2, maxIDLen);
			if(checkSingleID(ID2, maxIDLen, &accessIDs[0][0])){
				hwlib::cout << "Type the root password\n";
				currentArrayLocation++;
				if(!(pswd.setPassword(PWD[ (currentArrayLocation) ], maxLen, &currentArrayLocation))){
					hwlib::cout << "Something went wrong\n";
					currentArrayLocation--;
					for(int i = 0; i < 10; i++){
						ledRed.set(0);
						hwlib::wait_ms(100);
						ledRed.set(1);
						hwlib::wait_ms(100);
					}
					continue;
				}
				
				hwlib::cout << "\nPassword set\n";
				for(int i = 0; i<maxIDLen; i++){
					accessIDs[currentArrayLocation][i] = ID[i];
				}
				hwlib::cout << "ID set\n";
				ledRed.set(0);
				ledGreen.set(1);
				hwlib::wait_ms(1000);
				ledGreen.set(0);
				ledRed.set(1);
				int i = 0;
				while(PWD[currentArrayLocation][i] != '\0'){
					i++;
				}
				PWDLen[currentArrayLocation] = i+1;
				hwlib::wait_ms(100);
			}
			
			else{
				hwlib::cout << "Incorrect root tag\n";
				for(int i = 0; i < 15; i++){
					ledRed.set(0);
					hwlib::wait_ms(50);
					ledRed.set(1);
					hwlib::wait_ms(50);
				}
				continue;
			}
			for(int i = 0; i<maxIDLen; i++){
				hwlib::cout << (int)accessIDs[1][i] << '\n';
			}
			for(int i = 1; i<PWDLen[1]; i++){
				hwlib::cout << PWD[i] << '\n';
			}
		}
	}
	return 0;
}

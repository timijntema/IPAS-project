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

bool idChecker(byte * ID, int lenID, byte * accessIDs, int lenAccesIDs){
	for(int j = 0; j < lenAccesIDs; j++){
		if(checkRt(ID, lenID, accessIDs[j])){
			//put return indexes in array?
			return true;
		}
	}
	return false;
}

bool checkRt(byte * ID, int lenID, byte* rtID){
	bool checkID = true;
	for(int i = 0; i < lenID; i++){
		if(ID[i] != accessIDs[j][i]){
			return false;
		}
		if(i == (lenID-1) && checkID == true){
			return true;
		}
	}
}

int main(){
	WDT->WDT_MR = WDT_MR_WDDIS;
	//keypad variables
	char rootPWD[] = "9735";
	char userPWD[10][10] = {{*rootPWD}};
	int lenUserPWD = sizeof(userPWD)/sizeof(userPWD[0]);
	int lenRootPWD = sizeof(rootPWD)/sizeof(rootPWD[0]);
	char tempChar;
	
	//rfid
	const auto idLen = 5;
	byte ID[idLen];
	byte rootID[idLen] = {52,188,189,222,235};
	byte accessIDs[10][5] = {{*rootID}};
	bool checkID;
	
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
	
	//spi pins
	auto SDA = hwlib::target::pin_out(hwlib::target::pins::d8);
	auto CLK = hwlib::target::pin_out(hwlib::target::pins::d9);
	auto MOSI = hwlib::target::pin_out(hwlib::target::pins::d10);
	auto MISO = hwlib::target::pin_in(hwlib::target::pins::d11);
	auto RESET = hwlib::target::pin_out(hwlib::target::pins::d12);
	
	//keypad objects
	matrixKeypad keypad(keypad0, keypad1, keypad2, keypad3, keypad4, keypad5, keypad6, keypad7, buzzerPin);
	passwordOperations pswd(keypad, rootPWD, lenRootPWD, ledGreen, ledRed);
	
	//spi and RFID
	auto spiBusSetup = hwlib::spi_bus_bit_banged_sclk_mosi_miso(CLK, MOSI, MISO);
	mfrc522v2 RFID(spiBusSetup, SDA, RESET);
	
	//servo object
	servo servo1(servoPin);
	while(closePin.get() != 0){}
	hwlib::wait_ms(500);
	servo1.turnDegrees(145);
	
	while(1){
		checkID = true;
		ledRed.set(1);
		RFID.waitForCardID(ID, 5);
		ledRed.set(0);
		for(int i = 0; i < 5; i++){
			ledGreen.set(1);
			hwlib::wait_ms(50);
			ledGreen.set(0);
			hwlib::wait_ms(50);
		}
		ledRed.set(1);
		if(idChecker){
			
		}
		else{
			
		}
	}
	return 0;
}

/*
* File:   servo.cpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 10:04
*/

#include "servo.hpp"

servo::servo(hwlib::target::pin_out & pwmPin):
PWM_signal(pwmPin)
{}

void servo::turnDegrees(int degrees){
	if(degrees > MAX_DEGREES || degrees < MIN_DEGREES){
		return;
	}
	int pulseWidth = ((degrees*2170)/180);
	
	for(int i = 0; i<20; i++){
		PWM_pulse(pulseWidth);
	}
}
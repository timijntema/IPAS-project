/*
* File:   servo.cpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 10:04
*/

#include "servo.hpp"

servo::servo(PWM_signal & pwmSignal):
pwmSignal(pwmSignal)
{}

void servo::turnDegrees(int degrees){
	int pulseWidth = ((degrees*1000)/90);
	hwlib::cout << pulseWidth << '\n'; 
	
	for(int i = 0; i<20; i++){
		pwmSignal.PWM_pulse(pulseWidth);
	}
}
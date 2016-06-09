/*
* File:   PWM_signal.cpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 9:27
*/

#include "PWM_signal.hpp"

PWM_signal::PWM_signal(hwlib::target::pin_out & pwmPin):
pwmPin(pwmPin)
{}

void PWM_signal::PWM_pulse(int pulseWidth){
	pwmPin.set(1);
	hwlib::wait_us(pulseWidth);
	pwmPin.set(0);
	hwlib::wait_ms(20);
}

/*
* File:   PWM_signal.cpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 9:27
*/

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#include "PWM_signal.hpp"

PWM_signal::PWM_signal(hwlib::target::pin_out & pwmPin):
pwmPin(pwmPin)
{}

void PWM_signal::PWM_pulse(int pulseWidth){
	//One single pulse
	pwmPin.set(1);
	hwlib::wait_us(pulseWidth);
	pwmPin.set(0);
	hwlib::wait_ms(20);
}

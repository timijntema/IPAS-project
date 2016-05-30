/*
* File:   PWM_signal.hpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 9:27
*/

#ifndef PWM_SIGNAL_HPP
#define PWM_SIGNAL_HPP

#include "hwlib.hpp"

class PWM_signal{
protected:
	hwlib::target::pin_out & pwmPin;
public:
	PWM_signal(hwlib::target::pin_out & pwmPin);
	void PWM_pulse(int pulseWidth);
};

#endif // PWM_SIGNAL_HPP

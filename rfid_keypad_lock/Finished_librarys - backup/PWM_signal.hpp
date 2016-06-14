///@file

/*
* File:   PWM_signal.hpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 9:27
*/

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef PWM_SIGNAL_HPP
#define PWM_SIGNAL_HPP

#include "hwlib.hpp"

///Basic PWM signal
//
///This class is for creating a basic PWN signal object and
///then call seperate pules with a certain pulsewidth. It
/// uses a bit banged version of PWM.
class PWM_signal{
private:
	hwlib::target::pin_out & pwmPin;
public:

	///Default constructor
	//
	///This constructor sets up a pin to control using pwm.
	///It does this using the pin_out class in namespace hwlib.
	PWM_signal(hwlib::target::pin_out & pwmPin);
	
	///One pulse
	//
	///This function is made to create one PWM pulse with a user
	/// specified pulsewidth. The wait time between every pulse
	/// is about 20 ms.
	void PWM_pulse(int pulseWidth);
};

#endif // PWM_SIGNAL_HPP

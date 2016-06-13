///@file

/*
* File:   servo.hpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 10:04
*/

#ifndef SERVO_HPP
#define SERVO_HPP

#include "PWM_signal.hpp"
#include "hwlib.hpp"

//values can be different for other servo's
#define MAX_DEGREES 249
#define MIN_DEGREES 0

///Servo controll class
//
///This class is meant to act as a decorator class to the PWM_signal class.
///The idea is that it takes the pulse function and modifies it to send the
/// correct signal for a servo.
class servo : public PWM_signal{
public:
	
	///Default constructor
	//
	///This constructor gets a reference to pin on the Arduino Due. This
	/// pin is used to set up the PWM signal.
	servo(hwlib::target::pin_out & pwmPin);
	
	///Turn the servo to a amount of degrees
	//
	///This function get an amount of degrees and then it calculates the wait
	/// time. This wait time is beeing given to the pulse function. It also
	/// makes sure the servo gets more then one pulse to make sure it has
	/// enough time to turn to the set amount of degrees.
	void turnDegrees(int degrees);
};

#endif // SERVO_HPP

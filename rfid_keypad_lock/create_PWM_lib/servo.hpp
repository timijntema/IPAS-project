#ifndef SERVO_HPP
#define SERVO_HPP

#include "PWM_signal.hpp"
#include "hwlib.hpp"

class servo {
private:
	PWM_signal & pwmSignal;
public:
	servo(PWM_signal & pwmSignal);
	void turnDegrees(int degrees);
};

#endif // SERVO_HPP

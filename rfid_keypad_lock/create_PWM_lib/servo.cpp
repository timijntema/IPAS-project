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
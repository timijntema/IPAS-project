#include "hwlib.hpp"
#include "servo.hpp"

//add comments
//make decorator?
//use value max higher then 2
//check degrees for to high value

int main()
{
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	auto servoPin = hwlib::target::pin_out(hwlib::target::pins::d2);
	PWM_signal signal1(servoPin);
	servo servo1(signal1);
	
	while(1) {
		servo1.turnDegrees(0);
		hwlib::wait_ms(1000);
		servo1.turnDegrees(90);
		hwlib::wait_ms(1000);
	}
	
	return 0;
}

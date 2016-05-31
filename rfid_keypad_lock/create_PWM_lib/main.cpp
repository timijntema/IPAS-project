#include "hwlib.hpp"
#include "servo.hpp"

//make decorator?

//between 180 and 110 for the lock 

int main()
{
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	auto servoPin = hwlib::target::pin_out(hwlib::target::pins::d2);
	PWM_signal signal1(servoPin);
	servo servo1(signal1);
	
	while(1) {
		servo1.turnDegrees(180);
		hwlib::wait_ms(3000);
		servo1.turnDegrees(110);
		hwlib::wait_ms(3000);
	}
	
	return 0;
}

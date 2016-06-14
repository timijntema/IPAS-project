#include "hwlib.hpp"
#include "servo.hpp"

int main()
{
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	auto servoPin = hwlib::target::pin_out(hwlib::target::pins::d52);
	
	servo servo1(servoPin);
	while(1) {
		servo1.turnDegrees(180);
		hwlib::wait_ms(3000);
		servo1.turnDegrees(110);
		hwlib::wait_ms(3000);
	}
	
	return 0;
}

#include "hwlib.hpp"
#include "matrixKeypad.hpp"

int main()
{
	WDT->WDT_MR = WDT_MR_WDDIS;
	namespace target = hwlib::target;
	
	//column lines 37, 39, 41
	auto keypad0 = target::pin_in_out(target::pins::d32);
	auto keypad1 = target::pin_in_out(target::pins::d39);//34
	auto keypad2 = target::pin_in_out(target::pins::d41);//28
	auto keypad3 = target::pin_in_out(target::pins::d43);//29
	
	//row lines
	auto keypad4 = target::pin_in_out(target::pins::d45);
	auto keypad5 = target::pin_in_out(target::pins::d42);
	auto keypad6 = target::pin_in_out(target::pins::d44);
	auto keypad7 = target::pin_in_out(target::pins::d46);
	
	matrixKeypad keypad(keypad0, keypad1, keypad2, keypad3, keypad4, keypad5, keypad6, keypad7, 4, 4);
	
	while(1){
		hwlib::cout << keypad.getKey() << '\n';
		hwlib::wait_ms(300);
	}
	return 0;
}



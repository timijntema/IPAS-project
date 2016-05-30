#include "hwlib.hpp"
#include "matrixKeypad.hpp"

int main()
{
	WDT->WDT_MR = WDT_MR_WDDIS;
	namespace target = hwlib::target;
	
	//columns
	auto keypad0 = target::pin_in_out(target::pins::d23);
	auto keypad1 = target::pin_in_out(target::pins::d25);
	auto keypad2 = target::pin_in_out(target::pins::d27);
	auto keypad3 = target::pin_in_out(target::pins::d29);
	
	//rows
	auto keypad4 = target::pin_in_out(target::pins::d31);
	auto keypad5 = target::pin_in_out(target::pins::d33);
	auto keypad6 = target::pin_in_out(target::pins::d35);
	auto keypad7 = target::pin_in_out(target::pins::d30);
	
	matrixKeypad keypad(keypad0, keypad1, keypad2, keypad3, keypad4, keypad5, keypad6, keypad7, 4, 4);
	
	while(1){
		hwlib::cout << keypad.getKey() << '\n';
		hwlib::wait_ms(300);
	}
	return 0;
}



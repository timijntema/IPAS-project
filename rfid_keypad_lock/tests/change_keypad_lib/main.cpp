#include "hwlib.hpp"
#include "matrixKeypad.hpp"

int main()
{
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	//rows
	auto keypad0 = hwlib::target::pin_in_out(hwlib::target::pins::d32);
	auto keypad1 = hwlib::target::pin_in_out(hwlib::target::pins::d22);
	auto keypad2 = hwlib::target::pin_in_out(hwlib::target::pins::d24);
	auto keypad3 = hwlib::target::pin_in_out(hwlib::target::pins::d28);
	
	//columns
	auto keypad4 = hwlib::target::pin_in_out(hwlib::target::pins::d30);
	auto keypad5 = hwlib::target::pin_in_out(hwlib::target::pins::d34);
	auto keypad6 = hwlib::target::pin_in_out(hwlib::target::pins::d26);
	auto keypad7 = hwlib::target::pin_in_out(hwlib::target::pins::d31);//created fake pin because pin dummy ir creatint a wrong column problem
	
	auto buzzerPin = hwlib::target::pin_out(hwlib::target::pins::d6);
	
	matrixKeypad keypad(keypad0, keypad1, keypad2, keypad3, keypad4, keypad5, keypad6, keypad7, buzzerPin);
	
	while(1){
		hwlib::cout << keypad.getKey() << '\n';
		hwlib::wait_ms(300);
	}
	return 0;
}



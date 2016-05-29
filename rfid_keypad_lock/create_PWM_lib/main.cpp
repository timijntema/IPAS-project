#include "sam.h"
#include "hwlib.hpp"

void turn_oneeighty(hwlib::target::pin_out &servo){
	for (int i =0; i<20000; i++){
		servo.set(1);
		hwlib::wait_ms(0.05);//0.1
		servo.set(0);
		hwlib::wait_ms(0.9);
	}
}
void turn_degrees(hwlib::target::pin_out &servo, const int degrees){
	double y = (20000/180)*degrees/4;
	for (int i =0; i<y; i++){
		servo.set(1);
		hwlib::wait_ms(0.1);
		servo.set(0);
		hwlib::wait_ms(0.9);
	}
}

void turnservorevised(hwlib::target::pin_out &servo, const int degrees){
	/*for (int i = 0; i<(100*degrees); i++){
		servo.set(1);
		hwlib::wait_ms(0.01);//0.1
		servo.set(0);
		hwlib::wait_ms(1-0.01);
	}*/
	for (int i = 0; i<15000; i++){
		servo.set(1);
		hwlib::wait_ms(0.002);//0.1
		servo.set(0);
		hwlib::wait_ms(0.02);
	}
}

void pulse(double mark, double space)
{
    servo = 1;
    __delay32(mark * 30000.0);
    _LATD0 = 0;
    __delay32(space * 30000.0);
}

int main()
{
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	/*auto servo = hwlib::target::pin_out(hwlib::target::pins::d2);
	//turn_degrees(servo, 90);
	//turn_oneeighty(servo);
	turnservorevised(servo, 80);
	while(1){
	}
	return 0;*/
	int n;
    TRISD = 0;
   
    while(1)
    {
        for (n=0 ; n<100 ; ++n)  pulse(1 + (n/100.0), 19 - (n/100.0));
    }
   
    return 0;
}

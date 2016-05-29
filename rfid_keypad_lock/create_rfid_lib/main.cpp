#include "hwlib.hpp"

/*
 * Information on the rfid reader
 * 
 * up to 10mbit/s
 * Data on miso and mosi are send with MSB first
 * Data changed on the falling edge of the clock and stable on the rising edge (initial clock low??)
 * ss must be low to be able to send several bytes in one data stream. to send more
 * than one data stream ss must be set high between data streams
 */
 
int read_rfid(){
	auto SDA = hwlib::target::pin_out(hwlib::target::pins::d10);
	auto SCK = hwlib::target::pin_out(hwlib::target::pins::d9);
	//auto MOSI = hwlib::target::pin_out(hwlib::target::pins::d8);
	auto MISO = hwlib::target::pin_in(hwlib::target::pins::d7);
	
	SDA.set(0);
	int tmp = 0;
	for(int i1=1; i1 > 0; i1--){
		for(int i2 = 7; i2 >= 0; i2--){
			tmp = 0;
			SCK.set(0);
			hwlib::wait_ms(1);
			SCK.set(1);
			tmp = MISO.get();
			if (tmp == 1){
				hwlib::cout << "1";
				//tmp |=(1<<i2);
			}
			else{
				hwlib::cout << "0";
				//tmp &= ~(1<<i2);
			}
		}
		hwlib::cout << '\n';
	}
	SDA.set(1);
	SCK.set(1);
	return tmp;
}

int main()
{
	WDT->WDT_MR = WDT_MR_WDDIS;
	byte data_in[8];
	auto SDA = hwlib::target::pin_out(hwlib::target::pins::d10);
	auto SCK = hwlib::target::pin_out(hwlib::target::pins::d9);
	auto MOSI = hwlib::target::pin_out(hwlib::target::pins::d8);
	auto MISO = hwlib::target::pin_in(hwlib::target::pins::d7);
	
	auto RFID = hwlib::spi_bus_bit_banged_sclk_mosi_miso(SCK, MOSI, MISO);
	
	int result = 0;
	
	while(1){
		//read_rfid();
		//hwlib::wait_ms(2000);
		hwlib::cout << "reading\n";
		RFID.write_and_read(SDA, 8, nullptr, data_in);
		//for(auto &row : data_in){
			//result = (data_in[1] << 8) | data_in[0];
			//hwlib::cout << row << '\n';
			
			for (unsigned n = 0; n < sizeof( result ); n++)
				result = (result << 8) +data_in[ n ];
			//}
			hwlib::cout << "a: " << result << '\n';
		hwlib::cout << "\nend of bit set" << '\n';
		hwlib::wait_ms(2000);
		
	}
	return 0;
}
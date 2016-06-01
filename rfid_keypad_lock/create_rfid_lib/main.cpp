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
	auto MOSI = hwlib::target::pin_out(hwlib::target::pins::d8);
	auto MISO = hwlib::target::pin_in(hwlib::target::pins::d7);
	MOSI.set(1);
	SDA.set(0);
	hwlib::wait_ms(3);
	int tmp;
	for(int i1=1; i1 > 0; i1--){
		for(int i2 = 7; i2 >= 0; i2--){
			tmp = 0;
			SCK.set(0);
			hwlib::wait_ms(3);
			SCK.set(1);
			hwlib::wait_ms(3);
			tmp = MISO.get();
			hwlib::wait_ms(3);
			if (tmp == 1){
				hwlib::cout << "1";
				//tmp |=(1<<i2);
			}
			else{
				hwlib::cout << "0";
				//tmp &= ~(1<<i2);
			}
			hwlib::wait_ms(3);
		}
		hwlib::cout << '\n';
	}
	MOSI.set(0);
	SDA.set(1);
	SCK.set(1);
	return tmp;
}

int main()
{
	WDT->WDT_MR = WDT_MR_WDDIS;
	byte data_in[8];
	byte data_out[8] = {0b10010010, 0b10000010, 0b10000100, 0b10000110, 0b10001000, 0b10001010, 0b10001110};//, (0x09 << 1), (0x09 << 1), (0x09 << 1), (0x09 << 1), (0x09 << 1), (0x09 << 1)};
	auto SDA = hwlib::target::pin_out(hwlib::target::pins::d10);
	auto SCK = hwlib::target::pin_out(hwlib::target::pins::d9);
	auto MOSI = hwlib::target::pin_out(hwlib::target::pins::d8);
	auto MISO = hwlib::target::pin_in(hwlib::target::pins::d7);
	
	auto RFID = hwlib::spi_bus_bit_banged_sclk_mosi_miso(SCK, MOSI, MISO);
	int integer_in[78];
	
	while(1){
		hwlib::cout << "reading\n";
		RFID.write_and_read(SDA, 8, data_out, data_in);
		
		for(int j = 0; j < 8; j++){
			integer_in[j] = data_in[j];
			hwlib::cout << integer_in[j] << '\n';
		}
		hwlib::cout << "\nend of bit set" << '\n';
		hwlib::wait_ms(2000);
		
	}
	
	
	/*
	int result = 0;
	while(1){
		result = read_rfid();
		hwlib::cout << result << '\n';
		hwlib::wait_ms(2000);
	}
	*/
	return 0;
}
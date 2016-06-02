#include "hwlib.hpp"
#include "spiBus.hpp"
#include "mfrc522.hpp"

/*
 * Information on the rfid reader
 * 
 * up to 10mbit/s
 * Data on miso and mosi are send with MSB first
 * Data changed on the falling edge of the clock and stable on the rising edge (initial clock low??)
 * ss must be low to be able to send several bytes in one data stream. to send more
 * than one data stream ss must be set high between data streams
 */

int main()
{
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	auto SDA = hwlib::target::pin_out(hwlib::target::pins::d10);
	auto CLK = hwlib::target::pin_out(hwlib::target::pins::d9);
	auto MOSI = hwlib::target::pin_out(hwlib::target::pins::d8);
	auto MISO = hwlib::target::pin_in(hwlib::target::pins::d7);
	auto RESET = hwlib::target::pin_out(hwlib::target::pins::d6);
	
	spiBus spi(MOSI, MISO, CLK);
	mfrc522 RFID(spi, SDA, RESET);
	RFID.Init();
	
	byte data_in[8];
	byte data_out[9] = {RFID.FIFODataReg, (0x01 << 1), (0x02 <<1), RFID.FIFODataReg, RFID.FIFODataReg, RFID.FIFODataReg, RFID.FIFODataReg, RFID.FIFODataReg, 0x00};//, (0x09 << 1), (0x09 << 1), (0x09 << 1), (0x09 << 1), (0x09 << 1), (0x09 << 1)};
	
	//auto RFID = hwlib::spi_bus_bit_banged_sclk_mosi_miso(SCK, MOSI, MISO);
	int integer_in[10];
	
	while(1){
		hwlib::cout << "reading\n";
		RFID.readCardID(data_out, 9, data_in, 8);
		//RFID.write_and_read(SDA, 8, data_out, data_in);
		
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
#include "hwlib.hpp"
#include "spiBus.hpp"
#include "mfrc522v2.hpp"

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
	
	/*registers
	byte CommandReg = ((0x01 << 1) & 0x7E);
	byte FIFODataRegRead = (0x80 | ((0x09 << 1) & 0x7E));
	byte TModeReg = ((0x2A << 1) & 0x7E);
	byte TPrescalerReg = ((0x2B << 1) & 0x7E);
	byte TReloadRegH = ((0x2C << 1) & 0x7E);
	byte TReloadRegL = ((0x2D << 1) & 0x7E);
	byte TxASKReg = ((0x15 << 1) & 0x7E);
	byte ModeReg = ((0x11 << 1) & 0x7E);
	byte TxControlReg = ((0x14 << 1) & 0x7E);
	*/
	
	
	auto SDA = hwlib::target::pin_out(hwlib::target::pins::d10);
	auto CLK = hwlib::target::pin_out(hwlib::target::pins::d9);
	auto MOSI = hwlib::target::pin_out(hwlib::target::pins::d8);
	auto MISO = hwlib::target::pin_in(hwlib::target::pins::d7);
	auto RESET = hwlib::target::pin_out(hwlib::target::pins::d6);
	
	auto spiBusSetup = hwlib::spi_bus_bit_banged_sclk_mosi_miso(CLK, MOSI, MISO);
	mfrc522v2 RFID(spiBusSetup, SDA, RESET);
	
	byte ID[5];
	
	 while(1){
		RFID.waitForCardID(ID, 5);
		//hwlib::wait_ms(200);
	}
	return 0;
}
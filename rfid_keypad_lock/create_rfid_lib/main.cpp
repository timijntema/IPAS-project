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
	
	//registers
	//byte FIFODataReg = (0x09 << 1);
	byte TModeReg = (0x2A << 1);
	byte TPrescalerReg = (0x2B << 1);
	byte TReloadRegH = (0x2C << 1);
	byte TReloadRegL = (0x2D << 1);
	byte TxASKReg = (0x15 << 1);
	byte ModeReg = (0x11 << 1);
	byte TxControlReg = (0x14 << 1);
	
	
	
	auto SDA = hwlib::target::pin_out(hwlib::target::pins::d10);
	auto CLK = hwlib::target::pin_out(hwlib::target::pins::d9);
	auto MOSI = hwlib::target::pin_out(hwlib::target::pins::d8);
	auto MISO = hwlib::target::pin_in(hwlib::target::pins::d7);
	//auto RESET = hwlib::target::pin_out(hwlib::target::pins::d6);
	
	auto RFID = hwlib::spi_bus_bit_banged_sclk_mosi_miso(CLK, MOSI, MISO);
	
	/*
	 * Antenna
	 */
	
	byte data_out_settings[] = {TxControlReg, 0x00};
	byte data_in_settings[2];
	
	for(int i = 0; i < 1; i++){
		data_out_settings[i] = ((data_in_settings[i] << 1) & 0x7E);
	}
	
	byte send_back[] = {TxControlReg, 0x00, 0x00};
	
	RFID.write_and_read(SDA, 2, data_out_settings, data_in_settings);
	
	if ((data_in_settings[1] & 0x03) != 0x03){
		
		send_back[1] = (data_in_settings[1] | 0x03);
		for(int i = 0; i < 2; i++){
			send_back[i] = ((send_back[i] << 1) & 0x7E);
		}
		
		RFID.write_and_read(SDA, 3, send_back, nullptr);
		
	}
	
	/*
	 * Antenna end
	 */
	 
	 
	 byte settingRegister[] = {TModeReg, 0x80, TPrescalerReg, 0xA9, TReloadRegH, 0x03, TReloadRegL, 0xE8, TxASKReg, 0x40, ModeReg, 0x3D, 0x00};
	 for(int i = 0; i < 12; i++){
		settingRegister[i] = ((0x09 << 1) & 0x7E);
	}
	 
	RFID.write_and_read(SDA, 6, settingRegister, nullptr);
	
	/*
	 * Fifo register byte setup
	 */
	byte data_in[6];
	byte data_out[6];
	
	for(int i = 0; i < 5; i++){
		data_out[i] = ( 0x80 | ((0x09 << 1) & 0x7E));
	}
	data_out[5] = 0x00;
	
	/*
	 * end fifo register setup
	 */
	
	while(1){
		hwlib::wait_ms(100);
		RFID.write_and_read(SDA, 6, data_out, data_in);
		hwlib::wait_ms(1000);
		for(int i = 0; i < 6; i++){
			hwlib::cout << (int)data_in[i] << '\n';
		}
		hwlib::cout << "done\n\n";
		
	}
	
	/*spiBus spi(MOSI, MISO, CLK);
	mfrc522 RFID1(spi, SDA, RESET);
	//RFID.Init();
	
	
	byte data_in[9];
	byte data_out[9] = {RFID1.FIFODataReg, RFID1.FIFODataReg, RFID1.FIFODataReg, RFID1.FIFODataReg, RFID1.FIFODataReg, RFID1.FIFODataReg, RFID1.FIFODataReg, RFID1.FIFODataReg, 0x00};//, (0x09 << 1), (0x09 << 1), (0x09 << 1), (0x09 << 1), (0x09 << 1), (0x09 << 1)};
	
	auto RFID = hwlib::spi_bus_bit_banged_sclk_mosi_miso(CLK, MOSI, MISO);
	int integer_in[10];
	
	//setup
	
	byte settingRegisters[] = {RFID1.TModeReg, 0x80, RFID1.TPrescalerReg, 0xA9, RFID1.TReloadRegH, 0x03, RFID1.TReloadRegL, 0xE8, RFID1.TxASKReg, 0x40, RFID1.ModeReg, 0x3D, 0x00};//, TxControlReg};
	//byte setting[] = {0x80, 0xA9, 0x03, 0xE8, 0x40, 0x3D};//, 0x03};
	
	for (int i = 0; i < 12; i++){
			settingRegisters[i] = (settingRegisters[i] & 0x7E);
	}
	
	RFID.write_and_read(SDA, 13,  settingRegisters,nullptr);
	
	byte txAddr[2] = {((0x14 << 1) & 0x7E), 0x00};
	byte txReceived[2];
	
	RFID.write_and_read(SDA, 2, txReceived, txAddr);
	
	byte calc = ((txReceived[1]) | 0x03);
	byte txAddr2[3] = {RFID1.TxControlReg, calc, 0x00};
	
	if ((txReceived[1] & 0x03) != 0x03){
		RFID.write_and_read(SDA, 3, txAddr2, nullptr);
	}
	
	while(1){
		hwlib::cout << "reading\n";
		//RFID.readCardID(data_out, 9, data_in, 8);
		
		RFID.write_and_read(SDA, 9, data_out, data_in);
		
		for(int j = 0; j < 8; j++){
			integer_in[j] = data_in[j];
			hwlib::cout << integer_in[j] << '\n';
		}
		hwlib::cout << "\nend of bit set" << '\n';
		hwlib::wait_ms(2000);
	}
	
	
	int result = 0;
	while(1){
		result = read_rfid();
		hwlib::cout << result << '\n';
		hwlib::wait_ms(2000);
	}
	*/
	return 0;
}
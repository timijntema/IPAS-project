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
	
	byte backData[RFID.MAX_LEN];
	byte status;
	
	//RFID.reset();
	
	 while(1){
		//hwlib::cout << (int)(RFID.spiRead(RFID.CommandReg) & (1<<4)) << '\n';
		//hwlib::wait_ms(300);
		status = RFID.request(RFID.REQIDL, backData);
		//hwlib::cout << (int)status << '\n';
		status = RFID.anticoll(backData);
		//hwlib::cout << "start\n";
		//for(byte & a: backData){
		//	hwlib::cout << (int)a << '\n';
		//}
		//hwlib::cout << "end\n\n";
		//hwlib::cout << (int)status << '\n';
		if(status == RFID.MI_OK){
			hwlib::cout << "Card detected\n";
			hwlib::wait_ms(1000);
		}
		hwlib::wait_ms(2000);
	}
	/* 
	 byte settingRegister[] = {TModeReg, 0x80, TPrescalerReg, 0xA9, TReloadRegH, 0x03, TReloadRegL, 0xE8, TxASKReg, 0x40, ModeReg, 0x3D, 0x00};
	 
	RFID.write_and_read(SDA, 6, settingRegister, nullptr);
	
	 * Antenna
	 
	
	byte data_out_settings[] = {TxControlReg, 0x00};
	byte data_in_settings[2];
	byte send_back[] = {TxControlReg, 0x00, 0x00};
	
	RFID.write_and_read(SDA, 2, data_out_settings, data_in_settings);
	
	if ((data_in_settings[1] & 0x03) != 0x03){
		
		send_back[1] = (data_in_settings[1] | 0x03);
		hwlib::cout << "txcontrolreg not set\n";
		RFID.write_and_read(SDA, 3, send_back, nullptr);
		
	}
	
	
	 * Antenna end
	 
	
	
	 * Fifo register byte setup
	 
	byte data_in[8];
	byte data_out[8] = {0b00001100, CommandReg, FIFODataRegRead, FIFODataRegRead, FIFODataRegRead, FIFODataRegRead, FIFODataRegRead, 0X00};
	
	
	 * end fifo register setup
	 
	while(1){
		hwlib::wait_ms(100);
		RFID.write_and_read(SDA, 8, data_out, data_in);
		hwlib::wait_ms(1000);
		for(int i = 0; i < 8; i++){
			hwlib::cout << (int)data_in[i] << '\n';
		}
		hwlib::cout << "done\n\n";
		
	}
	*/
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
/*
* File:   mfrc522v2.hpp
* Author: Tim IJntema
*
* Created on 5 Juni 2016, 15:09
*/

#ifndef MFRC522V2_HPP
#define MFRC522V2_HPP

#include "hwlib.hpp"

class mfrc522v2 {
private:
	hwlib::spi_bus_bit_banged_sclk_mosi_miso & spi;
	hwlib::target::pin_out & SDA;
	hwlib::target::pin_out & RESET;
public:
	//maximum length
	int MAX_LEN = 16;
	
	//value's
	byte IDLE = 0X00;
	byte MEM = 0x01;
	byte RANDID = 0x02;
	byte CALCCRC = 0x03;
	byte TRANSMIT = 0X04;
	byte NOCMDCHANGE = 0X07;
	byte RECEIVE = 0x08;
	byte TRANSCEIVE = 0x0C;
	byte SOFTRESET = 0x0F;
	byte MFAUTHENT = 0x0E;
	byte ANTICOLL = 0X93;
	byte REQIDL = 0x26;
	
	//request checks
	byte MI_OK = 0;//0x00
	byte MI_NOTAGERR = 1;
	byte MI_ERR = 2;
	
	//Setup all registers (for spi on the mfrc522 the LSB is not used so the adresses have to be shifted to the left 1 spot.
	//						The MSB of the adress is used for indicating if you are trying to read or write, so thats why &0x7E is used)
	byte CommandReg = ((0x01 << 1) & 0x7E);
	byte FIFODataReg = ((0x09 << 1) & 0X7E);
	byte TModeReg = ((0x2A << 1) & 0X7E);
	byte TPrescalerReg = ((0x2B << 1) & 0X7E);
	byte TReloadRegH = ((0x2C << 1) & 0X7E);
	byte TReloadRegL = ((0x2D << 1) & 0X7E);
	byte TxASKReg = ((0x15 << 1) & 0X7E);
	byte ModeReg = ((0x11 << 1) & 0X7E);
	byte TxControlReg = ((0x14 << 1) & 0X7E);
	byte BitFramingReg = ((0x0D << 1) & 0x7E);
	byte ComIEnReg = ((0x02 << 1) & 0x7E);
	byte ComIrqReg = ((0x04 << 1) & 0x7E);
	byte FIFOLevelReg = ((0x0A << 1) & 0x7E);
	byte ErrorReg = ((0x06 << 1) & 0x7E);
	byte ControlReg = ((0x0C << 1) & 0x7E);
	
	
	mfrc522v2(hwlib::spi_bus_bit_banged_sclk_mosi_miso & spi, hwlib::target::pin_out & SDA, hwlib::target::pin_out & RESET);
	void init();
	void reset();
	void spiWrite(byte reg, byte value);
	byte spiRead(byte addr);
	void antennaOn();
	void setBitMask(byte addr, byte mask);
	void clearBitMask(byte addr, byte mask);
	byte request(byte mode, byte * backData);
	void toCard(byte value, byte * sendData, int lenSendData, byte * cardRetValue, byte * backData);
	byte anticoll(byte * backData);
	void waitForCardID(byte * ID, int lenID);
};

#endif // MFRC522V2_HPP

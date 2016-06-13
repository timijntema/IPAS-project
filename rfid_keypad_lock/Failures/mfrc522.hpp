#ifndef MFRC522_HPP
#define MFRC522_HPP

#include "hwlib.hpp"
#include "spiBus.hpp"

class mfrc522 {
private:
	spiBus & dataBus;
	hwlib::target::pin_out & SDA;
	hwlib::target::pin_out & RESET;
public:
	//registers
	byte CommandReg = ((0x01 << 1) & 0x7E);
	byte FIFODataReg = (0x09 << 1);
	byte TModeReg = (0x2A << 1);
	byte TPrescalerReg = (0x2B << 1);
	byte TReloadRegH = (0x2C << 1);
	byte TReloadRegL = (0x2D << 1);
	byte TxASKReg = (0x15 << 1);
	byte ModeReg = (0x11 << 1);
	byte TxControlReg = (0x14 << 1);
	
	mfrc522(spiBus & dataBus, hwlib::target::pin_out & SDA, hwlib::target::pin_out & RESET);
	void Init();
	void readCardID(byte * addr, int addrLen, byte * dataIn, int dataLen);
};

#endif // MFRC522_HPP

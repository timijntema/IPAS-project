#ifndef SPIBUS_HPP
#define SPIBUS_HPP

#include "hwlib.hpp"

class spiBus {
private:
	hwlib::target::pin_out & MOSI;
	hwlib::target::pin_in & MISO;
	hwlib::target::pin_out & CLK;
public:
	spiBus(hwlib::target::pin_out & MOSI, hwlib::target::pin_in & MISO, hwlib::target::pin_out & CLK);
	void sendByte(byte data);
	byte receiveByteFromAddr(byte addr);
	void receiveByteFromAddr(byte * addrArray, int addrLen, byte * dataIn, int dataLen);
};

#endif // SPIBUS_HPP

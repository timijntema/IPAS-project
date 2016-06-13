///@file
/*
* File:   RFID.hpp
* Author: Tim IJntema
*
* Created on 13 Juni 2016, 11:58
*/

#ifndef RFID_HPP
#define RFID_HPP

#include "hwlib.hpp"

class RFID{
protected:
	hwlib::spi_bus_bit_banged_sclk_mosi_miso & spi;
	hwlib::target::pin_out & SDA;
public:
	RFID(hwlib::spi_bus_bit_banged_sclk_mosi_miso & spi, hwlib::target::pin_out & SDA);
	virtual void init() = 0;
	virtual void reset() = 0;
	virtual void spiWrite(byte reg, byte value);
	virtual byte spiRead(byte addr);
	virtual void setBitMask(byte addr, byte mask);
	virtual void clearBitMask(byte addr, byte mask);
	virtual void toCard(byte value, byte * sendData, int lenSendData, byte * cardRetValue, byte * backData) = 0;
	virtual void waitForCardID(byte * ID, int lenID) = 0;
};

#endif // RFID_HPP

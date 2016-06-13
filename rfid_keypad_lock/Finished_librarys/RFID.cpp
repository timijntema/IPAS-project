///@file
/*
* File:   RFID.cpp
* Author: Tim IJntema
*
* Created on 13 Juni 2016, 11:58
*/


#include "RFID.hpp"

RFID::RFID(hwlib::spi_bus_bit_banged_sclk_mosi_miso & spi, hwlib::target::pin_out & SDA):
spi(spi),
SDA(SDA)
{}

void RFID::reset(byte addr, byte resetValue){
	spiWrite(addr, resetValue);//Execute the softreset command
}

void RFID::spiWrite(byte reg, byte value){
	byte temp[2] = {reg, value};
	spi.write_and_read(SDA, 2, temp, nullptr);
}

byte RFID::spiRead(byte addr){
	addr = (addr | 0x80); //The adresses need to have the 1st bit to 1 for reading (might be specific to the mfrc522)
	byte addrTemp[2] = {addr, 0x00};
	byte temp[2];//For the return values
	spi.write_and_read(SDA, 2, addrTemp, temp);
	return temp[1];//Return the value gotten from the mfrc522. Only the 2nd item is a return value the first is random data
}

void RFID::setBitMask(byte addr, byte mask){
	spiWrite(addr, ((spiRead(addr)) | mask));//Turn the mask on
}

void RFID::clearBitMask(byte addr, byte mask){
	spiWrite(addr, (spiRead(addr) & (~mask)));//Turn the mask off
}
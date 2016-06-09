/*
* File:   mfrc522v2.cpp
* Author: Tim IJntema
*
* Created on 5 Juni 2016, 15:09
*/

#include "mfrc522v2.hpp"

mfrc522v2::mfrc522v2(hwlib::spi_bus_bit_banged_sclk_mosi_miso & spi, hwlib::target::pin_out & SDA, hwlib::target::pin_out & RESET):
spi(spi),
SDA(SDA),
RESET(RESET)
{
	init();
}

void mfrc522v2::init(){
	SDA.set(1);
	RESET.set(1);//turn the hardware reset on
	
	//reset the mfrc522 unit
	reset();
	
	//set necessary registers
	spiWrite(TModeReg, 0x8D);//define the settings for the internal timer and the higher bits of the timer frequency
	spiWrite(TPrescalerReg, 0x3E);//define the lower 8 bits of the total 12 bit timer frequency
	spiWrite(TReloadRegL, 30);//set the timer reload value. (lower bits)
	spiWrite(TReloadRegH, 0);//set the timer reload value. (higher bits)
	spiWrite(TxASKReg, 0x40);//set transmit modulation settings. (forcing 100% ask modulation)
	spiWrite(ModeReg, 0x3D);//define transmitting and receiving settings
	
	antennaOn();//after the reset the antenna is off
}

void mfrc522v2::reset(){
	spiWrite(CommandReg, SOFTRESET);//execute the softreset command
}

void mfrc522v2::spiWrite(byte reg, byte value){ //maybe pointer?
	byte temp[2] = {reg, value};
	spi.write_and_read(SDA, 2, temp, nullptr);
}

byte mfrc522v2::spiRead(byte addr){//possible mistake of only sending 1 item
	addr = (addr | 0x80); //the adresses need to have the 1st bit to 1 for reading
	byte addrTemp[2] = {addr, 0x00};
	byte temp[2];//for return values
	spi.write_and_read(SDA, 2, addrTemp, temp);
	return temp[1];//return the value gotten from the mfrc522
}

void mfrc522v2::antennaOn(){
	byte temp = spiRead(TxControlReg);
	if (~(temp & 0x03)){//check if the antenna is on already
		setBitMask(TxControlReg, 0x03);//turn it on
	}
}

void mfrc522v2::setBitMask(byte addr, byte mask){//?possible improvements?
	byte temp = spiRead(addr);
	spiWrite(addr, (temp | mask));//turn the mask on
}

void mfrc522v2::clearBitMask(byte addr, byte mask){
	byte temp = spiRead(addr);
	spiWrite(addr, (temp & (~mask)));//turn the mask off
}

byte mfrc522v2::request(byte mode, byte * backData){//backdata needs to be max_len long to prevent length errors
	byte tagType[1] = {mode};
	
	spiWrite(BitFramingReg, 0x07);//define the amount of bits of the last byte that will be transmitted
	byte cardRetValue[2];//The return values are (status, backBitsLen)
	toCard(TRANSCEIVE, tagType, 1, cardRetValue, backData);//run the toCard function in transceive mode
	if ((cardRetValue[0] != MI_OK) || (cardRetValue[1] != 0x10)){//check if the data came back correctly
		cardRetValue[0] = MI_ERR;
	}
	return cardRetValue[0];//return status
}

void mfrc522v2::toCard(byte value, byte * sendData, int lenSendData, byte * cardRetValue, byte * backData){
	byte status = MI_ERR;
	byte irqEn = 0x00;
	byte waitIRq = 0x00;
	
	if(value == MFAUTHENT){//can possibly go because of not using it
		irqEn = 0x12;
		waitIRq = 0x10;
	}
	else if(value == TRANSCEIVE){
		irqEn = 0x77;
		waitIRq = 0x30;
	}
	
	spiWrite(ComIEnReg, (irqEn|0x80));//enable passing of the necessary interupt requests
	clearBitMask(ComIrqReg, 0x80);//set interrupt request bits 
	setBitMask(FIFOLevelReg, 0x80);//move the fifolevelreg pointer to a different spot in the fifo buffer
	
	spiWrite(CommandReg, IDLE);//make the mfrc522 idle
	
	for(int i = 0; i < lenSendData; i++){
		spiWrite(FIFODataReg, sendData[i]);//send the data to the fifo data register
	}
	
	spiWrite(CommandReg, value);//execute either the mfauthent command or the tranceive command
								//transeive for sending and reading data from the chip in the RFID tag
								//mfauthent for authenticating the RFID tag
	
	if (value == TRANSCEIVE){
		setBitMask(BitFramingReg, 0x80);//change the amount of bits transmitted from the last byte
	}
	
	byte temp;
	bool byte1;
	byte byte2;
	byte byte3;
	int i = 2000;
	while (1){
		temp = spiRead(ComIrqReg);//read the interrupt request bits
		i--;
		byte1 = (i!=0);
		byte2 = ~(temp&0x01);
		byte3 = ~(temp&waitIRq);
		if (~(byte1 and byte2 and byte3)){//bring the statement together
			break;
		}
		if (i == 0){//added to prevent a infinite loop
			status = MI_ERR;
			break;
		}
	}
	
	clearBitMask(BitFramingReg, 0x80);//change the amount of bits transmitted from the last byte
	
	byte backLen = 0;
	if (i != 0){
		if ((spiRead(ErrorReg) & 0x1B) == 0x00){//check the error register
			status = MI_OK;
			if (temp & irqEn & 0x01){
				status = MI_NOTAGERR;
			}
			if(value == TRANSCEIVE){
				temp = spiRead(FIFOLevelReg);//check where the pointer to the fifo data register is at to get the length of the tags data
				byte lastBits = (spiRead(ControlReg) & 0x07);//check if the first 3 bits of the control register are turned on to check for valid bits
				if (lastBits != 0){
					backLen = (temp-1)*8 + lastBits;
				}
				else{
					backLen = temp*8;
				}
				
				if (temp == 0){
					temp = 1;
				}
				else if (temp > MAX_LEN){
					temp = MAX_LEN;
				}
				i=0;
				while(i<temp){
					backData[i] = spiRead(FIFODataReg);//read the tags data from the fifo data register
					i++;
				}
			}
		}
		else{
			status = MI_ERR;
		}
	}
	
	
	cardRetValue[0] = status;
	cardRetValue[1] = backLen;
}

byte mfrc522v2::anticoll(byte * backData){
	byte cardRetValue[2];
	byte serNum[2] = {ANTICOLL, 0X20};//send anticolision data
	spiWrite(BitFramingReg, 0x00);//change the amount of bits transmitted from the last byte
	toCard(TRANSCEIVE, serNum, 2, cardRetValue, backData);
	auto serNumCheck = 0;
	if (cardRetValue[0] == MI_OK){
		if((cardRetValue[1]/8) == 5){
			int i = 0;
			while(i<4){
				serNumCheck = serNumCheck ^ backData[i];
				i++;
			}
			if (serNumCheck != backData[i]){
				cardRetValue[0] = MI_ERR;
			}
		}
		else{
			cardRetValue[0] = MI_ERR;
		}
	}
	
	return cardRetValue[0]; //return the status
}

void mfrc522v2::waitForCardID(byte * ID, int lenID){
	byte backData[MAX_LEN];
	byte status;
	while(1){
		status = request(REQIDL, backData);//get the status
		if(status == MI_OK){
			status = anticoll(backData);//get the ID
		}
		if(status == MI_OK){
			hwlib::cout << "Card detected\n";
			hwlib::cout << "ID: [" << (int)backData[0] << ',' << (int)backData[1] << ',' << (int)backData[2] << ',' << (int)backData[3] << ',' << (int)backData[4] << "]\n";
			break;
		}
	}
	for(int i = 0; i < lenID; i++){
		ID[i] = backData[i];//put the ID data in the ID array
	}
}
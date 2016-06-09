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
	RESET.set(1);//Turn the hardware reset pin on
	
	//Reset the mfrc522 unit
	reset();
	
	//Set necessary registers
	spiWrite(TModeReg, 0x8D);//Define the settings for the internal timer and the higher bits of the timer frequency
	spiWrite(TPrescalerReg, 0x3E);//Define the lower 8 bits of the total 12 bit timer frequency
	spiWrite(TReloadRegL, 30);//Set the timer reload value. (lower bits)
	spiWrite(TReloadRegH, 0);//Set the timer reload value. (higher bits)
	spiWrite(TxASKReg, 0x40);//Set transmit modulation settings. (forcing 100% ask modulation)
	spiWrite(ModeReg, 0x3D);//Define transmitting and receiving settings
	
	antennaOn();//After the reset the antenna is off
}

void mfrc522v2::reset(){
	spiWrite(CommandReg, SOFTRESET);//Execute the softreset command
}

void mfrc522v2::spiWrite(byte reg, byte value){
	byte temp[2] = {reg, value};
	spi.write_and_read(SDA, 2, temp, nullptr);
}

byte mfrc522v2::spiRead(byte addr){
	addr = (addr | 0x80); //The adresses need to have the 1st bit to 1 for reading
	byte addrTemp[2] = {addr, 0x00};
	byte temp[2];//For the return values
	spi.write_and_read(SDA, 2, addrTemp, temp);
	return temp[1];//Return the value gotten from the mfrc522. Only the 2nd item is a return value the first is random data
}

void mfrc522v2::antennaOn(){
	byte temp = spiRead(TxControlReg);
	if (~(temp & 0x03)){//Check if the antenna is on already
		setBitMask(TxControlReg, 0x03);//Turn it on
	}
}

void mfrc522v2::setBitMask(byte addr, byte mask){
	spiWrite(addr, ((spiRead(addr)) | mask));//Turn the mask on
}

void mfrc522v2::clearBitMask(byte addr, byte mask){
	spiWrite(addr, (spiRead(addr) & (~mask)));//Turn the mask off
}

byte mfrc522v2::request(byte mode, byte * backData){//BackData needs to be 2 bytes long
	spiWrite(BitFramingReg, 0x07);//Define the amount of bits of the last byte that will be transmitted
	byte cardRetValue[2];//The return values are (status, backBitsLen)
	toCard(TRANSCEIVE, &mode, 1, cardRetValue, backData);//Run the toCard function in transceive mode
	if ((cardRetValue[0] != MI_OK) || (cardRetValue[1] != 0x10)){//Check if the data came back correctly
		cardRetValue[0] = MI_ERR;
	}
	return cardRetValue[0];//Return status
}

void mfrc522v2::toCard(byte value, byte * sendData, int lenSendData, byte * cardRetValue, byte * backData){
	byte status = MI_ERR;
	byte irqEn = 0x77;
	byte waitIRq = 0x30;
	
	spiWrite(ComIEnReg, (irqEn|0x80));//Enable passing of the necessary interupt requests
	clearBitMask(ComIrqReg, 0x80);//Set interrupt request bits 
	setBitMask(FIFOLevelReg, 0x80);//Move the fifolevelreg pointer to a different spot in the fifo buffer
	
	spiWrite(CommandReg, IDLE);//Make the mfrc522 idle
	
	for(int i = 0; i < lenSendData; i++){
		spiWrite(FIFODataReg, sendData[i]);//Send the data to the fifo data register
	}
	
	spiWrite(CommandReg, value);//Execute the value command in the commandreg
	
	setBitMask(BitFramingReg, 0x80);//Change the amount of bits transmitted from the last byte
	
	byte temp;
	bool byte1;
	byte byte2;
	byte byte3;
	int i = 2000;
	while (i<0){
		temp = spiRead(ComIrqReg);//Read the interrupt request bits
		i--;
		byte1 = (i!=0);
		byte2 = ~(temp&0x01);
		byte3 = ~(temp&waitIRq);
		if (~(byte1 and byte2 and byte3)){//Bring the statement together
			break;
		}
	}
	
	clearBitMask(BitFramingReg, 0x80);//Change the amount of bits transmitted from the last byte
	
	byte backLen = 0;
	if (i != 0){
		if ((spiRead(ErrorReg) & 0x1B) == 0x00){//Check the error register
			status = MI_OK;
			if (temp & irqEn & 0x01){
				status = MI_NOTAGERR;
			}
			temp = spiRead(FIFOLevelReg);//Check where the pointer to the fifo data register is at to get the length of the tags data
			byte lastBits = (spiRead(ControlReg) & 0x07);//Check if the first 3 bits of the control register are turned on to check for valid bits
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
				backData[i] = spiRead(FIFODataReg);//Read the tags data from the fifo data register
				i++;
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
	byte serNum[2] = {ANTICOLL, 0X20};//Put collision check data in the array
	spiWrite(BitFramingReg, 0x00);//Change the amount of bits transmitted from the last byte
	toCard(TRANSCEIVE, serNum, 2, cardRetValue, backData);
	auto serNumCheck = 0;
	if (cardRetValue[0] == MI_OK){
		if((cardRetValue[1]/8) == 5){ //Change from amount of bits to amount of bytes and then check
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
	while(1){
		if(request(REQIDL, ID) == MI_OK){
			if(anticoll(ID) == MI_OK){//Get the id
				hwlib::cout << "Card detected\n";
				hwlib::cout << "ID: [" << (int)ID[0] << ',' << (int)ID[1] << ',' << (int)ID[2] << ',' << (int)ID[3] << ',' << (int)ID[4] << "]\n";
				break;
			}
		}
	}
}
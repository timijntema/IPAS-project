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
	RESET.set(1);
	
	//reset the mfrc522 unit
	reset();
	
	//set necessary registers
	spiWrite(TModeReg, 0x8D);
	spiWrite(TPrescalerReg, 0x3E);
	spiWrite(TReloadRegL, 30);
	spiWrite(TReloadRegH, 0);
	spiWrite(TxASKReg, 0x40);
	spiWrite(ModeReg, 0x3D);
	
	antennaOn();
}

void mfrc522v2::reset(){
	spiWrite(CommandReg, SOFTRESET);
}

void mfrc522v2::spiWrite(byte reg, byte value){ //maybe pointer?
	byte temp[2] = {reg, value};
	spi.write_and_read(SDA, 2, temp, nullptr);
	//delete temp; //remove if it dusn't work
}

byte mfrc522v2::spiRead(byte addr){//possible mistake of only sending 1 item
	addr = (addr | 0x80);
	byte addrTemp[2] = {addr, 0x00};
	byte temp[2];
	spi.write_and_read(SDA, 2, addrTemp, temp);
	return temp[1];
}

void mfrc522v2::antennaOn(){
	byte temp = spiRead(TxControlReg);
	if (~(temp & 0x03)){
		setBitMask(TxControlReg, 0x03);
	}
}

void mfrc522v2::setBitMask(byte addr, byte mask){//?possible improvements?
	byte temp = spiRead(addr);
	spiWrite(addr, (temp | mask));
}

void mfrc522v2::clearBitMask(byte addr, byte mask){
	byte temp = spiRead(addr);
	spiWrite(addr, (temp & (~mask)));
}

byte mfrc522v2::request(byte mode, byte * backData){//backdata needs to be max_len long
	//byte status = 0x00;
	//byte retBits = 0x00;
	byte tagType[1] = {mode};
	
	spiWrite(BitFramingReg, 0x07);
	byte cardRetValue[2];//status, backBits
	toCard(TRANSCEIVE, tagType, 1, cardRetValue, backData);\
	if ((cardRetValue[0] != MI_OK) || (cardRetValue[1] != 0x10)){
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
	
	spiWrite(ComIEnReg, (irqEn|0x80));
	clearBitMask(ComIrqReg, 0x80);
	setBitMask(FIFOLevelReg, 0x80);
	
	spiWrite(CommandReg, IDLE);
	
	for(int i = 0; i < lenSendData; i++){
		spiWrite(FIFODataReg, sendData[i]);
	}
	
	spiWrite(CommandReg, value);
	
	if (value == TRANSCEIVE){
		setBitMask(BitFramingReg, 0x80);
	}
	
	byte temp;
	bool byte1;
	byte byte2;
	byte byte3;
	int i = 2000;
	while (1){
		temp = spiRead(ComIrqReg);
		i--;
		byte1 = (i!=0);
		byte2 = ~(temp&0x01);
		byte3 = ~(temp&waitIRq);
		if (~(byte1 and byte2 and byte3)){//converted a ~ into ! in the whole line
			break;
		}
	}
	
	clearBitMask(BitFramingReg, 0x80);
	
	byte backLen = 0;
	if (i != 0){
		if ((spiRead(ErrorReg) & 0x1B) == 0x00){
			status = MI_OK;
			if (temp & irqEn & 0x01){//dusnt go in here
				status = MI_NOTAGERR;
			}
			if(value == TRANSCEIVE){
				temp = spiRead(FIFOLevelReg);
				byte lastBits = (spiRead(ControlReg) & 0x07);
				//hwlib::cout<<(int)temp <<'\n';
				if (lastBits != 0){
					backLen = (temp-1)*8 + lastBits;
				}
				else{
					backLen = temp*8;//why *8
				}
				
				if (temp == 0){
					temp = 1;
				}
				else if (temp > MAX_LEN){
					temp = MAX_LEN;
				}
				i=0;
				while(i<temp){
					backData[i] = spiRead(FIFODataReg);
					//hwlib::cout << (int)backData[i] << '\n';
					//hwlib::cout << (int)temp << '\n';
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
	byte serNum[2] = {ANTICOLL, 0X20};
	spiWrite(BitFramingReg, 0x00);
	toCard(TRANSCEIVE, serNum, 2, cardRetValue, backData);
	auto serNumCheck = 0;
	if (cardRetValue[0] == MI_OK){
		if((cardRetValue[1]/8) == 5){
			int i = 0;
			while(i<4){//why not 5?
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
	
	return cardRetValue[0]; //status
}

void mfrc522v2::waitForCardID(byte * ID, int lenID){
	byte backData[MAX_LEN];
	byte status;
	while(1){
		status = request(REQIDL, backData);
		if(status == MI_OK){
			status = anticoll(backData);
		}
		if(status == MI_OK){
			hwlib::cout << "Card detected\n";
			hwlib::cout << "ID: [" << (int)backData[0] << ',' << (int)backData[1] << ',' << (int)backData[2] << ',' << (int)backData[3] << ',' << (int)backData[4] << "]\n";
			//hwlib::wait_ms(300);
			break;
		}
	}
	for(int i = 0; i < lenID; i++){
		ID[i] = backData[i];
	}
}
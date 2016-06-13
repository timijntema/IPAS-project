#include "mfrc522.hpp"

mfrc522::mfrc522(spiBus & dataBus, hwlib::target::pin_out & SDA, hwlib::target::pin_out & RESET):
dataBus(dataBus),
SDA(SDA),
RESET(RESET)
{}

void mfrc522::Init(){
	RESET.set(1);
	SDA.set(0);
	
	byte settingRegister[] = {TModeReg, TPrescalerReg, TReloadRegH, TReloadRegL, TxASKReg, ModeReg};//, TxControlReg};
	byte setting[] = {0x80, 0xA9, 0x03, 0xE8, 0x40, 0x3D};//, 0x03};
	byte settingAddr;
	
	for (int i = 0; i < 6; i++){
		settingAddr = (settingRegister[i] & 0x7E);
		dataBus.sendByte(settingAddr);
		dataBus.sendByte(setting[i]);
	}
	
	settingAddr = dataBus.receiveByteFromAddr(TxControlReg);
	if ((settingAddr & 0x03) != 0x03){
		dataBus.sendByte(TxControlReg);
		dataBus.sendByte((settingAddr | 0x03));
	}
	SDA.set(1);
}

void mfrc522::readCardID(byte * addr, int addrLen, byte * dataIn, int dataLen){
	SDA.set(0);
	
	byte sendAddr = (0x80 | (addr[0] & 0x7E)); //spi on this device uses the MSB to set the read or write and the LSB is always a logic 0
	dataBus.sendByte(sendAddr);//bit operaties
	for(int i = 0; i < dataLen; i++){
		if ((i+1) >= addrLen){
			SDA.set(1);
			return;
		}
		
		sendAddr = (0x80 | (addr[(i+1)] & 0x7E));
		dataIn[i] = dataBus.receiveByteFromAddr(sendAddr);
	}
	SDA.set(1);
}
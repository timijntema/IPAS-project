#include "spiBus.hpp"

spiBus::spiBus(hwlib::target::pin_out & MOSI, hwlib::target::pin_in & MISO, hwlib::target::pin_out & CLK):
MOSI(MOSI),
MISO(MISO),
CLK(CLK)
{
	CLK.set(0);
}

void spiBus::sendByte(byte data){
	for(int i = 0; i < 8; i++){
		CLK.set(0);
		MOSI.set((data & 0x80) !=0);
		data = data << 1;
		hwlib::wait_ms(2);
		CLK.set(1);
		hwlib::wait_ms(2);
	}
	CLK.set(0);
}

byte spiBus::receiveByteFromAddr(byte addr){
	int tmp;
	byte returnByte = 0b00000000;
	
	for(int i = 0; i < 8; i++){
		tmp = 0;
		CLK.set(0);
		MOSI.set((addr & 0x80) !=0 );
		//hwlib::cout << "\n\n" << (integer_in & 0x80) << "\n\n";
		addr = addr << 1;
		hwlib::wait_ms(2);
		CLK.set(1);
		hwlib::wait_ms(2);
		tmp = MISO.get();
		
		if (tmp == 1){
			//hwlib::cout << tmp;
			returnByte |= (1<<(8-i));
		}
		else{
			//hwlib::cout << tmp;
			returnByte &= ~(1<<(8-i));
		}
		hwlib::wait_ms(1);
	}
	hwlib::cout << '\n';
	CLK.set(0);
	return returnByte;
}

void spiBus::receiveByteFromAddr(byte * addrArray, int addrLen, byte * dataIn, int dataLen){
	for(int i = 0; i < addrLen; i++){
		dataIn[i] = receiveByteFromAddr(addrArray[i]);
	}
}

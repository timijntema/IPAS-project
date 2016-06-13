///@file
/*
* File:   mfrc522.hpp
* Author: Tim IJntema
*
* Created on 5 Juni 2016, 15:09
*/

#ifndef MFRC522_HPP
#define MFRC522_HPP

#include "hwlib.hpp"

///Reading RFID tags with mfrc522
//
///This class can be used to read the UID of an RFID tag. It uses another class created by
/// Wouter van Ooijen. The other class is used for reading from and writing values to the
/// GPIO pins on the Arduino Due. This library is a recreation from a python original that
/// can be found on this link: https://github.com/mxgxw/MFRC522-python.
class mfrc522 {
private:
	hwlib::spi_bus_bit_banged_sclk_mosi_miso & spi;
	hwlib::target::pin_out & SDA;
	hwlib::target::pin_out & RESET;
public:
	//maximum length
	int MAX_LEN = 16;
	
	//value's
	const byte IDLE = 0X00;
	const byte TRANSCEIVE = 0x0C;
	const byte SOFTRESET = 0x0F;
	const byte MFAUTHENT = 0x0E;
	const byte ANTICOLL = 0X93;
	const byte REQIDL = 0x26;
	
	//request checks
	const byte MI_OK = 0;
	const byte MI_NOTAGERR = 1;
	const byte MI_ERR = 2;
	
	//Setup all registers
	//(for spi on the mfrc522 the LSB is not used so the addresses have to be shifted to the left 1 spot.
	//The MSB of the address is used for indicating if you are trying to read or write, so thats why &0x7E is used)
	const byte CommandReg = ((0x01 << 1) & 0x7E);
	const byte FIFODataReg = ((0x09 << 1) & 0X7E);
	const byte TModeReg = ((0x2A << 1) & 0X7E);
	const byte TPrescalerReg = ((0x2B << 1) & 0X7E);
	const byte TReloadRegH = ((0x2C << 1) & 0X7E);
	const byte TReloadRegL = ((0x2D << 1) & 0X7E);
	const byte TxASKReg = ((0x15 << 1) & 0X7E);
	const byte ModeReg = ((0x11 << 1) & 0X7E);
	const byte TxControlReg = ((0x14 << 1) & 0X7E);
	const byte BitFramingReg = ((0x0D << 1) & 0x7E);
	const byte ComIEnReg = ((0x02 << 1) & 0x7E);
	const byte ComIrqReg = ((0x04 << 1) & 0x7E);
	const byte FIFOLevelReg = ((0x0A << 1) & 0x7E);
	const byte ErrorReg = ((0x06 << 1) & 0x7E);
	const byte ControlReg = ((0x0C << 1) & 0x7E);
	
	///Default constructor
	//
	///The constructor takes three items from the hwlib namespace, the already created spi interface,
	/// the ss or chip select line that is attached to the SDA pin of the mfrc522 and the RESET pin.
	///The RESET pin is for hard resetting the mfrc522.
	mfrc522(hwlib::spi_bus_bit_banged_sclk_mosi_miso & spi, hwlib::target::pin_out & SDA, hwlib::target::pin_out & RESET);
	
	///Initialization of the mfrc522
	//
	///This function contains al the setting that are needed to be done for the mfrc522 to send the
	/// correct information. It is beeing called by the constructor automatically when the constructor
	/// is beeing called. For more information about the different settings, check the cpp file.
	///Comments have been added to every line that send something to the mfrc522.
	void init();
	
	///Reset the mfrc522
	//
	///This function executes a soft reset on the mfrc522 using the previously created SOFTRESET byte.
	///It sends this command to the command register using the spiWrite function.
	void reset();
	
	///Write values to the mfrc522
	//
	///This function writes values to the mfrc522 using the bit banged spi bus from the hwlib namespace.
	///It takes a register to write the value to and the value itself. It puts these bytes in a byte array.
	///The byte array gets send to the spi but from the hwlib namespace as 2 bytes of information that have
	/// to be send.
	void spiWrite(byte reg, byte value);
	
	///Read values from the mfrc522
	//
	///This function reads values from a register that has been specified when the function is beeing called.
	///Because the adresses from the spi interface need a 1 on the MSB spot to identify that information is
	/// beeing read, the address is beeing changed using a or operator (addr | 0x80). After this two byte arrays
	/// are created. One for the outgoing adresses and the other one for the received information. The function
	/// returns the received value as one byte.
	byte spiRead(byte addr);
	
	///Turn the antenna on
	//
	///After a softreset the antenna for reading the RFID tags is turned off. To turn it on we have the antennaOn
	/// function. It reads the TxControlRegister and checks the returned value to see if the antenna is still on.
	/// If it is not on the setBitMask function turns it on.
	void antennaOn();
	
	///Set a bitmask
	//
	///This function reads the given address and then turns the mask bits in the return value on. After doing this
	/// it sends back the new data to the same adress.
	void setBitMask(byte addr, byte mask);
	
	///Clear a bitmask
	//
	///This function turns certain bits of in the specified address and keeps the rest the same. It dus this by reading
	/// the specified register and then using a and operator on the returned value. The mask itself needs to be inverted
	/// to be correct.
	void clearBitMask(byte addr, byte mask);
	
	///Send data request
	//
	///This function sends a data request to the RFID tag. If we get MI_OK back and the length of the return data of the tag is equal
	/// to 0x10 (in bits), we can start getting the UID data. That is done by the anticoll function. This function returns the status.
	byte request(byte mode, byte * backData);
	
	///Send data to the RFID tag and get the returned information
	//
	///We send data to the RFID tag and get the return data back from the tag. The data that comes back is beeing returned
	/// using the cardRetValue and backData parameters. backData contains the bytes of data returned by the tag.
	///cardRetValue contains the status and the length of the returned data.
	void toCard(byte value, byte * sendData, int lenSendData, byte * cardRetValue, byte * backData);
	
	///Check for collision and return the UID
	//
	///This function checks for collision errors and returns the UID of a tag. The return value is a byte that shows the
	/// status. If the return value == MI_ERR of MI_NOTAGERR then something went wrong. The data it returns is 5 bytes long.
	byte anticoll(byte * backData);
	
	///Wait for a tag to be found and read it
	//
	///This function waits for a new RFID tag to be found. If it is found the function reads the UID of the tag.
	///The function returns the UID. using a ID parameter on the function call. This ID parameter is a byte array
	/// that is 5 bytes long. Notice: the function dus not have an actual return value, it uses the ID parameter to
	/// return the UID.
	void waitForCardID(byte * ID, int lenID);
};

#endif // MFRC522V2_HPP

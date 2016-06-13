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
#include "RFID.hpp"

///Reading RFID tags with mfrc522
//
///This class can be used to read the UID of an RFID tag. It uses another class created by
/// Wouter van Ooijen. The other class is used for reading from and writing values to the
/// GPIO pins on the Arduino Due. This library is a recreation from a python original that
/// can be found on this link: https://github.com/mxgxw/MFRC522-python.
class mfrc522 : public RFID{
private:
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
	void init() override;
	
	///Turn the antenna on
	//
	///After a softreset the antenna for reading the RFID tags is turned off. To turn it on we have the antennaOn
	/// function. It reads the TxControlRegister and checks the returned value to see if the antenna is still on.
	/// If it is not on the setBitMask function turns it on.
	void antennaOn();
	
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
	void toCard(byte value, byte * sendData, int lenSendData, byte * cardRetValue, byte * backData) override;
	
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
	void waitForCardID(byte * ID, int lenID) override;
};

#endif // MFRC522_HPP

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

///RFID superclass
//
///This is a superclass for RFID readers. It contains some basic functions an RFID reader needs to operate.
///Some of the functions in this class are abstract because the implementation will be different for different
/// RFID readers.
class RFID{
protected:
	hwlib::spi_bus_bit_banged_sclk_mosi_miso & spi;
	hwlib::target::pin_out & SDA;
public:
	///Default constructor
	//
	///This constructor takes a spi bus and a select pin as parameter. It sets the protected variables using
	/// these parameters. The protected variables can be used by the RFID class or any other class that 
	/// inherits the RFID class.
	RFID(hwlib::spi_bus_bit_banged_sclk_mosi_miso & spi, hwlib::target::pin_out & SDA);
	
	///Initialize the RFID reader
	//
	///This function is meant to initialize the RFID reader. It can be used to set any settings needed to
	/// succesfully read an RFID tag. The function will have a different meaning for every RFID reader.
	///It will be dependent on the company who made the RFID reader.
	virtual void init() = 0;
	
	///Soft reset the reader
	//
	///This funtion is meant to make a soft reset easy to do. The register that has to be set will be different
	/// for every RFID reader. But in the end they all have to send some value to an register address.
	virtual void reset(byte addr, byte resetValue);
	
	///Write values to the mfrc522
	//
	///This function writes values to the RFID reader using the bit banged spi bus from the hwlib namespace.
	///It takes a register to write the value to and the value itself. It puts these bytes in a byte array.
	///The byte array gets send to the spi but from the hwlib namespace as 2 bytes of information that have
	/// to be send.
	virtual void spiWrite(byte reg, byte value);
	
	///Read values from RFID reader
	//
	///This function reads values from a register that has been specified when the function is beeing called.
	///Because the adresses from the spi interface need a 1 on the MSB spot to identify that information is
	/// beeing read, the address is beeing changed using a or operator (addr | 0x80). After this two byte arrays
	/// are created. One for the outgoing adresses and the other one for the received information. The function
	/// returns the received value as one byte.
	virtual byte spiRead(byte addr);
	
	///Set a bitmask
	//
	///This function reads the given address and then turns the mask bits in the return value on. After doing this
	/// it sends back the new data to the same adress.
	virtual void setBitMask(byte addr, byte mask);
	
	///Clear a bitmask
	//
	///This function turns certain bits of in the specified address and keeps the rest the same. It dus this by reading
	/// the specified register and then using a and operator on the returned value. The mask itself needs to be inverted
	/// to be correct.
	virtual void clearBitMask(byte addr, byte mask);
	
	///Send data to tag
	//
	///This function is meant to be used to send data to the RFID tag and if necessary return the data. It takes data to send
	/// and the length of this data. There also need to be given 2 byte arrays to return certain data. The first is meant to
	/// return a status for the data. So if there is an error it will be in the cardRetValue array on index 0. The second item
	/// in the first array is the length of backData. BackData will contain every bit of data the RFID reader read off the tag.
	virtual void toCard(byte value, byte * sendData, int lenSendData, byte * cardRetValue, byte * backData) = 0;
	
	///return a tag UID
	//
	///This function waits for a tag to be held against the RFID reader. It returns the UID of that tag. It returns the UID using
	/// the byte array ID. The implementation will be different for different RFID readers.
	virtual void waitForCardID(byte * ID, int lenID) = 0;
};

#endif // RFID_HPP

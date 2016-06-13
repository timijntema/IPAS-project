/*
* File:   accessOperations.hpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 17:13
*/

#ifndef ACCESSOPERATIONS_HPP
#define ACCESSOPERATIONS_HPP

#include "matrixKeypad.hpp"
#include "hwlib.hpp"

///Operations on accessing the box
//
///This class is meant to make operations on passwords and with passwords
/// a lot more easy. It uses the matrixKeypad library to obtain the passwords.
///It also has functions for checking the RFID UID's.
class accessOperations{
private:
	matrixKeypad & keypad;
	int lenRootPWD;
	char * rootPWD;
	hwlib::pin_out & ledGreen;
	hwlib::pin_out & ledRed;
public:
	///Default constructor
	//
	///The constructor of this class takes a reference to the matrix keypad to
	/// make sure we can use all the functions of the keypad. It also takes a
	/// root password meant for changing the user password(s)
	accessOperations(matrixKeypad & keypad, char * rootPWD, int lenRootPWD, hwlib::pin_out & ledGreen = hwlib::pin_out_dummy, hwlib::pin_out & ledRed = hwlib::pin_out_dummy);
	
	///Get and check passwords
	//
	///This function is meant to take a user password and check it against the
	/// input it gets from the keypad library. If the input is incorrect it
	/// gives you 2 more tries. (for security purposes a long waiting time could
	/// be added after 3 wrong tries) The function gives true back if the password
	/// was correct and false if the password was incorrect.
	bool getPassword(char * clientPWD, int lenCharArray);
	
	///Set a new client password
	//
	///This function can be used to change a clients password. It first checks the root
	/// password using the getPassword() function and if it is correct it will ask you
	/// for the new client password. For that is uses the matrix keypad library. If the
	/// root password was not filled in correctly the function will return false. If
	/// everything word out correctly the function returns true.
	bool setPassword(char * clientPWD, const int & lenCharArray, int *currentArrayLocation);
	
	///Check one UID
	//
	///This function can be used to check one single UID from an RFID tag. The function
	/// requires the ID itself, the length of the ID and the ID to compare the first ID with.
	///It returns true if the 2 ID's match.
	bool checkSingleID(byte * ID, int lenID, byte * checkID);
	
	///Check more than one UID
	//
	///The first parameter of this function is the ID that has to be compared to the rest.
	///The length of the ID to compare has to be given as well. The third and fourth parameter
	/// are the multidimensional array filled with UID's (or not depending on the amount of users)
	/// and the length of that array (first index). The function returns two values. One is a
	/// boolean for if statement use and the seccond one is the array location. This location is a
	/// parameter that indicates what the index is the ID is found on in the multidimensional array.
	bool checkMultipleID(byte * ID, int lenID, int lenAccesIDs, byte (*accessIDs)[5], int * arrayLocation);
};

#endif // ACCESSSOPERATIONS_HPP

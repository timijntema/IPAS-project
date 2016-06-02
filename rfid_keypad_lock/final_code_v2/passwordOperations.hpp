/*
* File:   passwordOperations.hpp
* Author: Tim IJntema
*
* Created on 30 may 2016, 17:13
*/

#ifndef PASSWORDOPERATIONS_HPP
#define PASSWORDOPERATIONS_HPP

#include "matrixKeypad.hpp"
#include "hwlib.hpp"

///Operations on passwords
//
///This class is meant to make operations on passwords and with passwords
/// a lot more easy. It uses the matrixKeypad library to obtain the passwords.
class passwordOperations{
private:
	matrixKeypad & keypad;
	int lenRootPWD;
	char * rootPWD;
public:
	///Default constructor
	//
	///The constructor of this class takes a reference to the matrix keypad to
	/// make sure we can use all the functions of the keypad. It also takes a
	/// root password meant for changing the user password(s)
	passwordOperations(matrixKeypad & keypad, char * rootPWD, int lenRootPWD);
	
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
	bool setPassword(char * clientPWD, int lenCharArray);
};

#endif // PASSWORDOPERATIONS_HPP

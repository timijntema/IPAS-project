///@file
/*
* File:   matrixKeypad.hpp
* Author: Tim IJntema
*
* Created on 24 april 2016, 16:54
*/

#ifndef MATRIXKEYPAD_HPP
#define	MATRIXKEYPAD_HPP

#include "hwlib.hpp"
#include <string>

///Obtain pressed keys from 4x4 keypad
//
///This class that can be used to obtain pressed keys from a keypad using an Arduino Uno board.
///The seperate keypresses can be obtained or an array of characters can be filled with pressed characters.
///It uses a second class i created to use the Arduino Uno pins.
class matrixKeypad{
private:
	int keypadRow = -1;
	int keypadColumn = -1;
	
	hwlib::pin_in_out * pinColumn[4];
	hwlib::pin_in_out * pinRow[4];
	
	int rowsize = 4;
	int columnsize = 4;
	
	//The multidimensional array containing all the characters used by the keypad
	char keypad[4][4] = {
		{'1', '2', '3', 'A'},
		{'4', '5', '6', 'B'},
		{'7', '8', '9', 'C'},
		{'*', '0', '#', 'D'}
	};
	
public:
	///Default constructor
	//
	///This constructor sets up all the pins on the Arduino Uno.
	///It calls the constructor from the pinsetup class and sends the bitnumbers and letters to specifi wich port is wich.
	matrixKeypad( hwlib::pin_in_out & p0,
				hwlib::pin_in_out & p1,
				hwlib::pin_in_out & p2,
				hwlib::pin_in_out & p3,
				hwlib::pin_in_out & p4,
				hwlib::pin_in_out & p5,
				hwlib::pin_in_out & p6,
				hwlib::pin_in_out & p7 = hwlib::pin_in_out_dummy,
				int rowsize = 4,
				int columnsize = 4);
	
	///Obtain seperate keys
	//
	///This function optains the seperate keys from the keypad.
	///It waits for a key to be pressed and then returns the pressed key as an character.
	///It knows what key was pressed by seting up 4 inputs and 4 outputs. The inputs have a
	/// pullup resistor. After that it checks for a row that is pulled down to 0. After 
	/// that it has the pressed row. To know the pressed column, it switches the inputs to outputs
	/// and the outputs to inputs. That way it can check the pressed column. 
	char getKey();
	
	///Obtain a string
	//
	///This function uses the getKey() function to obtain the seperate keys. 
	///It puts these pressed keys in a user defined array with a user defined array length.
	///If the maximum length of the array -1 (for the '\0' character) has been reached,
	/// it protects you from going out of the array. To stop with getting characters 
	/// before the array is full you have to press the '#' character. This will stops the
	/// process and adds a '\0' after the last character.
	void getString(std::string & tmpString);
};


#endif	/* MATRIXKEYPAD_HPP */


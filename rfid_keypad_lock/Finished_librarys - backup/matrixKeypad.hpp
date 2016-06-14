///@file
/*
* File:   matrixKeypad.hpp
* Author: Tim IJntema
*
* Created on 27 may 2016, 14:36
*/

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef MATRIXKEYPAD_HPP
#define	MATRIXKEYPAD_HPP

#include "hwlib.hpp"

///Obtain pressed keys from a keypad
//
///This is a class that can be used to obtain pressed keys from a keypad using an Arduino Due board.
///The seperate keypresses can be obtained or an array of characters can be filled with pressed characters.
///It uses another namespace containing classes that make it capable of using Arduino Due pins. This second
/// namespace has been made by Wouter Ooijen and not by me.
class matrixKeypad{
private:
	int keypadRow = -1;
	int keypadColumn = -1;
	
	hwlib::pin_in_out * pinColumn[4];
	hwlib::pin_in_out * pinRow[4];
	
	const int rowSize = 4;
	int colSize;
	
	hwlib::pin_out & buzzerPin;
	
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
	///This constructor sets up all the pins on the Arduino Due. It takes the size of
	/// the keypad and the pins on wich it is attached. The last pin is setup with a dummy
	/// as default value to ensure that 4x3 keypads dont give errors. It als takes a buzzerpin
	/// for giving a little sound when a key is pressed. This pin also has a dummy as default.
	///This makes sure the buzzer is not actually needed.
	matrixKeypad( hwlib::pin_in_out & p0,
				hwlib::pin_in_out & p1,
				hwlib::pin_in_out & p2,
				hwlib::pin_in_out & p3,
				hwlib::pin_in_out & p4,
				hwlib::pin_in_out & p5,
				hwlib::pin_in_out & p6,
				hwlib::pin_in_out & p7 = hwlib::pin_in_out_dummy,
				int colSize = 3,
				hwlib::pin_out & buzzerPin = hwlib::pin_out_dummy
				);
	
	///Obtain seperate keys
	//
	///This function optains the seperate keys from the keypad.
	///It waits for a key to be pressed and then returns the pressed key as an character.
	///It knows what key was pressed by seting up 4 inputs and 4(or 3) outputs. The inputs have a
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
	/// process and adds a '\0' after the last character. The function returns the array
	/// size usable with for loops to go through the array and execute operations on the array.
	int getString(char * chararray, int lenCharArray);
};


#endif	/* MATRIXKEYPAD_HPP */


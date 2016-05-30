/*
* File:   matrixKeypad.hpp
* Author: Tim IJntema
*
* Created on 24 april 2016, 16:54
*/

#include "matrixKeypad.hpp"

matrixKeypad::matrixKeypad( hwlib::pin_in_out & p0,
							hwlib::pin_in_out & p1,
							hwlib::pin_in_out & p2,
							hwlib::pin_in_out & p3,
							hwlib::pin_in_out & p4,
							hwlib::pin_in_out & p5,
							hwlib::pin_in_out & p6,
							hwlib::pin_in_out & p7,
							int rowsize,
							int columnsize):
pinColumn{&p3, &p2, &p1, &p0},
pinRow{&p7, &p6, &p5, &p4},
rowsize(rowsize),
columnsize(columnsize)
{}

char matrixKeypad::getKey(){
	while (1){
		//Set al the columns as output and turn them off.
		for (int i = 0; i <  columnsize; i++){
			(pinColumn[i])->direction_set_output();
			(pinColumn[i])->set(0);
		}
		
		//Set the rows as input with pullup resistor.
		for (int i = 0; i <  rowsize; i++){
			(pinRow[i])->direction_set_input();
			//(pinRow[i]).enablePud();
		}
		
		//Scan the rows for a pushed button.
		//KeypadRow should be between 0 and 3 if any number or letter on the keypad is pressed.
		keypadRow = -1;
		for (int i = 0; i <  rowsize; i++){
			if ((pinRow[i])->get() == 0){
				keypadRow = i;
			}
		}
		
		//Check if anything is pressed. if not just continue and skip the rest of the code.
		if (keypadRow < 0 || keypadRow > 3){
			hwlib::wait_ms(500);
			continue;
		}
		
		//hwlib::cout << keypadColumn << '\n';
		//hwlib::cout << keypadRow << '\n';
		
		//hwlib::cout<<keypadRow << " and col: " << keypadColumn << "colsize: " << columnsize << '\n';
		
		//Set the columns as input with pull up resistor.
		for (int i = 0; i <  columnsize; i++){
			(pinColumn[i])->direction_set_input();
			//(pinColumn[i]).enablePud();
		}
		
		(pinRow[keypadRow])->direction_set_output();
		(pinRow[keypadRow])->set(0);
		
		//Scan the columns for the pushed button.
		//KeypadColumn should be between 0 and 3 as well.
		keypadColumn = -1;
		for (int i = 0; i <  columnsize; i++){
			if ((pinColumn[i])->get() == 0){
				keypadColumn = i;
			}
		}
		
		
		//Check for a number unequal to -1 on both the column and row.
		if ((keypadRow != -1) && (keypadColumn != -1)){
			//Return the pressed keypad character from the multidimensional array.
			return keypad[keypadRow][keypadColumn];
		}
		hwlib::wait_ms(500);
	}
}

void matrixKeypad::getString(std::string & tmpString){
	char pressedKey;
	int i = 0;
	
	//Continue to check for pressed keys until the # character has been pressed.
	while((pressedKey = getKey()) != '#'){
		tmpString[i] = pressedKey;
		i++;
		//Wait time.
		hwlib::wait_ms(300);
	}
	tmpString[i] = '\0';//Add the \0 to mark the end of the set of characters that has been obtained.
}
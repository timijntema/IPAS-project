//! @file
//! This library implements a bit-banged output-only UART at
//! the baudrate specified by BMPTK_BAUDRATE.

#ifndef NINCLUDE_SW_UART_H
#define NINCLUDE_SW_UART_H

//! initialize the UART 
//
//! Call this function to initialize the UART pins.
void uart_init( void );

//! write a single character
//
//! This function writes a single character using the standard UART
//! output pin of the chip (GP1_7) and the baudrate BMPTK_BAUDRATE,
//! which is passed as a command line argument during compilation.
void uart_put_char( char c );

//! write a character string
//
//! This function writes the characters of a standard 0-terminated string
//! using uart_put_char().
void uart_put_string( const char *s );

//! writes an integer in decimal format without leading zero's.
//
//! This function writes an integer in decimal format, without
//! any leading zero's, using using uart_put_char().
//! A negative value will have a '-' in front of it.
void uart_put_int_decimal( int x );

//! writes an unsigend integer as n hexadecimal digits
//
//! This function writes an unsigend integer as n hexadecimal (0-F) digits.
//! Digits higher than n will be ignored.
void uart_put_int_hexadecimal( unsigned int x, int n );	

#endif

// ==========================================================================
//
// File      : hwlib-due.hpp
// Part of   : hwlib library for V1OOPC and V1IPAS
// Copyright : wouter@voti.nl 2016
//
// hwlib implementation for an Arduino Due (ATSAM3X8E chip)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================
/// @file

#ifndef HWLIB_DUE_H
#define HWLIB_DUE_H

#include "hwlib-all.hpp"
#include "sam.h"

/// hwlib implementation for the Arduino Due
//
/// \image html due-pcb.jpg
//
/// This namespace contains the hwlib implementation of the pins, timing
/// and (sotware) UART output for the Arduino Due (ATSAM3X8E chip).
/// The first wait call configures the chip to run at 12 MHz, 
/// from its internal (calibrated) RC oscillator.
///
/// The port and pin parameters to the constructors of the pin classes
/// can either use to the ATSAM3X8E ports and pins, or the Arduino names.
///
/// The Due has an on-board orange LED connected to port 1 pin 27.
///
/// The chip runs at 3.3 Volt and that is the level on its IO pins.
///
/// \image html due-pinout.png
///
/// The ATSAM3X8E chip has a watchdog system that is enabled by default.
/// If left alone, the watchdog will reset the chip after a short time. 
/// To prevent this, all Due applications start with these lines:
/// \code{.cpp}
/// // kill the watchdog
/// WDT->WDT_MR = WDT_MR_WDDIS;
/// \endcode
///
/// References:
///    - <A HREF="https://www.arduino.cc/en/uploads/Main/arduino-uno-schematic.pdf">
///       Arduino Due circuit reference diagram</A> (pdf)
///    - <A HREF="http://www.atmel.com/images/atmel-11057-32-bit-cortex-m3-microcontroller-sam3x-sam3a_datasheet.pdf">
///       ATSAM38XE datasheet</A> (pdf)
///
namespace due {

/// Arduino Due pin names
//
/// These are the pins of an Arduino Due board.
/// Digital pins d0..d13, analog input pins A0..A5, 
/// SCL, SDA, TX (=D1), RX (=D0), 
/// LED (=D13), SCK (=D13), MISO (=D12), MOSI (=D11), SS (=D10).
enum class pins {
   d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, 
   d11, d12, d13, d14, d15, d16, d17, d18, d19, d20,
   d21, d22, d23, d24, d25, d26, d27, d28, d29, d30,
   d31, d32, d33, d34, d35, d36, d37, d38, d39, d40,
   d41, d42, d43, d44, d45, d46, d47, d48, d49, d50,
   d51, d52, d53, 
   a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11,
   dac0, dac1, canrx, cantx,
   scl, sda, scl1, sda1,
   tx, rx, led, 
   sck, miso, mosi, cs0, cs1
/// \cond INTERNAL    
   ,SIZE_THIS_IS_NOT_A_PIN
/// \endcond   
};

/// \cond INTERNAL 

struct pin_info_type { 
   uint8_t port; 
   uint8_t pin; 
};  

const HWLIB_WEAK pin_info_type & pin_info( pins name ){
   
static const pin_info_type pin_info_array[ (int) pins::SIZE_THIS_IS_NOT_A_PIN ] = {
   { 0,  8 },  // d0
   { 0,  9 },  // d1
   { 1, 25 },  // d2
   { 2, 28 },  // d3
   { 2, 26 },  // d4
   { 2, 25 },  // d5
   { 2, 24 },  // d6
   { 2, 23 },  // d7
   { 2, 22 },  // d8
   { 2, 21 },  // d9
   { 2, 29 },  // d10
   
   { 3,  7 },  // d11
   { 3,  8 },  // d12
   { 1, 17 },  // d13
   { 3,  4 },  // d14
   { 3,  5 },  // d15
   { 0, 13 },  // d16
   { 0, 12 },  // d17
   { 0, 11 },  // d18
   { 0, 10 },  // d19
   { 1, 12 },  // d20
   
   { 1, 13 },  // d21
   { 1, 26 },  // d22
   { 0, 14 },  // d23
   { 0, 15 },  // d24
   { 3,  0 },  // d25
   { 3,  1 },  // d26
   { 3,  2 },  // d27
   { 3,  3 },  // d28
   { 3,  6 },  // d29
   { 3,  9 },  // d30

   { 0,  7 },  // d31
   { 3, 10 },  // d32
   { 2,  1 },  // d33
   { 2,  2 },  // d34
   { 2,  3 },  // d35
   { 3,  4 },  // d36
   { 3,  5 },  // d37
   { 3,  6 },  // d38
   { 3,  7 },  // d39
   { 3,  8 },  // d40

   { 3,  9 },  // d41
   { 0, 19 },  // d42
   { 0, 20 },  // d43
   { 2, 19 },  // d44
   { 2, 18 },  // d45
   { 2, 17 },  // d46
   { 2, 16 },  // d47
   { 2, 15 },  // d48
   { 2, 14 },  // d49
   { 2, 13 },  // d50
   
   { 2, 12 },  // d51
   { 1, 21 },  // d52
   { 1, 14 },  // d53
   
   { 0, 16 },  // a0
   { 0, 24 },  // a1
   { 0, 23 },  // a2
   { 0, 22 },  // a3
   { 0,  6 },  // a4
   { 0,  4 },  // a5
   { 0,  3 },  // a6
   { 0,  2 },  // a7
   { 1, 17 },  // a8
   { 1, 18 },  // a9
   { 1, 19 },  // a10
   { 1, 20 },  // a11
   
   { 1, 15 },  // dac0
   { 1, 16 },  // dac1
   { 0,  1 },  // cantx
   { 0,  0 },  // canrx
   
   { 1, 13 },  // scl
   { 1, 12 },  // sda
   { 0, 18 },  // scl1
   { 0, 17 },  // sda1
   
   { 0,  9 },  // tx
   { 0,  8 },  // rx
   { 1, 27 },  // led

   { 0, 27 },  // sck
   { 0, 25 },  // miso
   { 0, 26 },  // mosi
   { 2, 29 },  // cs0
   { 2, 26 }   // cs1
};    
   
   uint_fast8_t n = static_cast< uint_fast8_t>( name );
   if( n >= static_cast< uint_fast8_t>( pins::SIZE_THIS_IS_NOT_A_PIN )){
      HWLIB_PANIC_WITH_LOCATION;
   }
   return pin_info_array[ n ];
}
 
Pio & __attribute__((weak)) port_registers( int port ){
   // a bit of a cludge to put this here:
   // enable the clock to all GPIO ports
   PMC->PMC_PCER0 = ( 0x3F << 11 );

   switch( port ){
      case 0  : return *PIOA;
      case 1  : return *PIOB;
      case 2  : return *PIOC;
      case 3  : return *PIOD;
      default : break;
   }   
   HWLIB_PANIC_WITH_LOCATION; 
   // doesn't return
}

/// \endcond

/// pin_in implementation for a ATSAM3X8E
class pin_in : public hwlib::pin_in {
private:
   Pio & port;
   int mask;
   
public:

   /// Arduino Due pin_in constructor
   //
   /// Constructor for a ATSAM3X8E input pin.
   ///
   /// The port_number and pin_number refer to the chip, 
   /// not to the Arduino board pin names.
   ///
   /// This constructor sets the pin direction to input.
   pin_in( int port_number, int pin_number ): 
      port{ port_registers( port_number ) }, 
      mask{ 0x1 << pin_number }
   {
      port.PIO_ODR = mask;
   }
   
   /// Arduino Due pin_in constructor from a Due pin name
   //
   /// This call creates a pin_in from an Arduino Due pin name.
   ///
   /// This constructor sets the pin direction to input.
   pin_in( pins name ): 
      pin_in{ 
         pin_info( name ).port, 
         pin_info( name ).pin 
      }
   {}   
   
   bool get() override {
      return (( port.PIO_PDSR & mask ) != 0 );   
   }
};

/// pin_out implementation for a ATSAM3X8E
class pin_out : public hwlib::pin_out {
private:
   Pio & port;
   int mask;
   
public:

   /// Arduino Due pin_out constructor
   //
   /// Constructor for a ATSAM3X8E input pin.
   ///
   /// The port_number and pin_number refer to the chip, 
   /// not to the Arduino board pin names.
   ///
   /// This constructor sets the pin direction to output.
   pin_out( int port_number, int pin_number ): 
      port{ port_registers( port_number ) }, 
      mask{ 0x1 << pin_number }
   {
      port.PIO_OER  = mask; 
   }
   
   /// Arduino Due pin_out constructor from a Due pin name
   //
   /// This call creates a pin_out from an Arduino Due pin name.
   ///
   /// This constructor sets the pin direction to output.
   pin_out( pins name ): 
      pin_out{ 
         pin_info( name ).port, 
         pin_info( name ).pin 
      }
   {}     
   
   void set( bool v ) override {
      ( v 
         ?  port.PIO_SODR 
         :  port.PIO_CODR 
      )  = mask;
   }
};

/// pin_in_out implementation for a ATSAM3X8E
class pin_in_out : public hwlib::pin_in_out {
private:
   Pio & port;
   int mask;
   
public:

   /// Arduino Due pin_out constructor
   //
   /// Constructor for a ATSAM3X8E input pin.
   ///
   /// The port_number and pin_number refer to the chip, 
   /// not to the Arduino board pin names.
   ///
   /// This constructor doesn't set the pin direction 
   /// to input or output, a direction_set function must
   /// be called to do so.
   pin_in_out( int port_number, int pin_number ): 
      port{ port_registers( port_number ) }, 
      mask{ 0x1 << pin_number }
   {}
   
   /// Arduino Due pin_out constructor from a Due pin name
   //
   /// This call creates a pin_out from an Arduino Due pin name.
   ///
   /// This constructor doesn't set the pin direction 
   /// to input or output, a direction_set function must
   /// be called to do so.
   pin_in_out( pins name ): 
      pin_in_out{ 
         pin_info( name ).port, 
         pin_info( name ).pin 
      }
   {}    
   
   void direction_set_input() override {
      port.PIO_ODR = mask;
   }
   
   bool get() override {
      return ( port.PIO_PDSR & mask ) != 0;   
   }   
   
   void direction_set_output() override {
      port.PIO_OER  = mask;    
   }
   
   void set( bool v ) override {
      ( v 
         ?  port.PIO_SODR 
         :  port.PIO_CODR 
      )  = mask;
   }
};   

/// pin_oc implementation for a ATSAM3X8E
class pin_oc : public hwlib::pin_oc {
private:
   Pio & port;
   int mask;
   
public:

   /// Arduino Due pin_oc constructor
   //
   /// Constructor for a ATSAM3X8E input pin.
   ///
   /// The port_number and pin_number refer to the chip, 
   /// not to the Arduino board pin names.
   ///
   /// This constructor sets the pin to high (high-impedance). 
   pin_oc( int port_number, int pin_number ): 
      port{ port_registers( port_number ) }, 
      mask{ 0x1 << pin_number }
   {
      port.PIO_ODR = mask;
   }
   
   /// Arduino Due pin_oc constructor from a Due pin name
   //
   /// This call creates a pin_oc from an Arduino Due pin name.
   ///
   /// This constructor sets the pin to high (high-impedance). 
   pin_oc( pins name ): 
      pin_oc{ 
         pin_info( name ).port, 
         pin_info( name ).pin 
      }
   {}       
   
   bool get() override {
      // hwlib::cout << "=>" << (int) port.PIO_PDSR << "\n";
      return ( port.PIO_PDSR & mask ) != 0;   
   }   
   
   void set( bool v ) override {
      if( v ){
         port.PIO_ODR = mask;
      } else {
         port.PIO_OER  = mask;    
         port.PIO_CODR = mask;
      }
   }

};

}; // namespace due

namespace hwlib {

namespace target = ::due;
   
void HWLIB_WEAK wait_ns( int_fast32_t n ){
   wait_us( n / 1000 );
}

void HWLIB_WEAK wait_us( int_fast32_t n ){

   // use the 12 MHz internal RC clock
   PMC->CKGR_MOR = 0x00370028;
   
   // This code is for a clock frequency of 12 MHz:
   // the loop uses exactly 12 cycles on an M3
   // (1 cycle branch penalty due to branch target forwarding).
   // Note: use subs, sub on an M3 is the wide instruction
   // that doesn't set the status flags.
   __asm volatile( 
      "   mov r0, %[reg]   \t\n"
      "   b   1f           \t\n"
      "   .align 4         \t\n"
      "1: subs r0, #1      \t\n" 
      "   b   2f           \t\n"
      "2: b   3f           \t\n"
      "3: b   4f           \t\n"
      "4: b   5f           \t\n"
      "5: nop              \t\n"
      "   bgt 1b           \t\n" 
      :: [reg] "r" (n) : "r0"
   );
}

void HWLIB_WEAK wait_ms( int_fast32_t n ){
   while( n > 0 ){
      wait_us( 1000 );
      --n;
   }   
}   

void HWLIB_WEAK uart_putc( char c ){
   static target::pin_out pin( 0, 9 );
   uart_putc_bit_banged_pin( c, pin );
}

}; //namespace hwlib   

#endif // HWLIB_DUE_H

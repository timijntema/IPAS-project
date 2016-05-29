// blink a LED on pin Arduino 13 = Atmeag328P PB5
//
// This is the pin that is used in the very first Arduino example.
// Some (but not all) Arduino Uno boards have a built-in LED at this pin.
//
// This is a very simple blink-a-LED application.
// It aims to show direct use of the hardware rather than
// good programming practices (like abstracting from the GPIO pin
// used and using a timer for the delay).
//
// It uses a busy-wait for the delay, which can be affected
// by the compiler and optimization settings used.
//

// includes the definitions for the chip      
#include "bmptk.h"      

//volatile char x[ 10 ];

//volatile char y[ 33 ] = { 1,2,3,4,5,6,7,8,9,10 };   

//void __attribute__((noinline)) f(){
//   volatile char x[ 10 ];
//   x[ 0 ] = x[ 1 ];
///}
			
int main( void ){	
   // must be volatile to prevent optimization of the wait loops
   // note: use long, because int on an AVR is 16 bits
   volatile unsigned long int i;
    
   // make the pin an output
   DDRB |= ( 0x01 << 5 );
   
//   f();
//   f();
   
   // loop forever
   while( 1 ){
   
      // make the pin high
      PORTB |= ( 0x01 << 5 );

      // wait 
      for( i = 0; i < 150000; i++ ){ }
	
      // make the pin low
      PORTB &= ~( 0x01 << 5 );

      // wait 
      for( i = 0; i < 150000; i++ ){ }
	
	}     
}
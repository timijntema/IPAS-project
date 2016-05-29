// blink a LED on pin P0.31
// On the HU ARM V4 board this pin controls the green LED
// next to the second USB connector.
//
// This is a very simple blink-a-LED application.
// It aims to show direct use of the hardware rather than
// good programming practices (like abstracting from the GPIO pin
// used and using a timer for the delay).
//
// It uses the bmptk LPC2148 header file to access the direction
// and value registers for pin GPIO1_4.
// It uses a busy-wait for the delay, which can be affected
// by the compiler and optimization settings used.
//
// Note that after a reset the pin is automatically
// configured as GPIO, hence there is no need to configure
// the pin. Some other pins are NOT configured to GPIO by a reset,
// hence blinking on such a pin requires the additional step of
// configuring the pin as GPIO.
//
// This code uses the slow (legacy) GPIO registers.
// A serious application would probably use the fast GPIO registers.
//

// includes the cmsis definitions for the chip
#include "bmptk.h"
			
int main( void ){	

   // must be volatile to prevent optimization of the wait loops
   volatile int i;
    
   // make the pin an output
   IO0DIR = IO0DIR | ( 0x1 << 31 ); 
   
   // loop forever
   while( 1 ){
   
      // make the pin low
      IO0CLR = ( 0x1 << 31 );

      // wait 
      for( i = 0; i < 100000; i++ ){
      }
      
      // make the pin high
      IO0SET = ( 0x1 << 31 );

      // wait 
      for( i = 0; i < 100000; i++ ){ 
      }
	
	}     
}
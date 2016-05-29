// blink a LED on pin P1.0 (red LED)A
//
// This is a very simple blink-a-LED application.
// It aims to show direct use of the hardware rather than
// good programming practices (like abstracting from the GPIO pin
// used and using a timer for the delay).
//
// It uses the CMSIS header file to access the direction
// and value registers for pin GPIO1_4.
// It uses a busy-wait for the delay, which can be affected
// by the compiler and optimization settings used.
//
// Note that after a reset the GPIO1_4 pin is automatically
// configured as GPIO, hence there is no need to configure
// the pin. Some other pins are NOT configured to GPIO by a reset,
// hence blinking on such a pin requires the additional step of
// configuring the pin as GPIO.
//

// includes the definitions for the chip 
#include "bmptk.h"

// http://www.yuriystoys.com/2013/09/how-to-upload-firmware-to-msp430.html
			
int main( void ){	

   // must be volatile to prevent optimization of the wait loops
   volatile int i;
   
   // stop the watchdog
    WDTCTL = WDTPW + WDTHOLD; 
    
   // make the pin an output
   P1DIR |= ( 0x1 << 0 );
   
   // loop forever
   while( 1 ){
   
      // make the pin high
      P1OUT |= ( 0x1 << 0 );

      // wait 
      for( i = 0; i < 100000; i++ ){ 
      }
	
      // make the pin low
      P1OUT &= ~( 0x1 << 0 );

      // wait 
      for( i = 0; i <50000; i++ ){ 
      }
	
	}     
}

// blink a LED on pin PTB19
// (= green part of the three-color LED on the board)
//
// This is a very simple blink-a-LED application.
// It aims to show direct use of the hardware rather than
// good programming practices (like abstracting from the pin
// used and using a timer for the delay).
//
// It uses the CMSIS header file to access the direction
// and value registers for pin.
// It uses a busy-wait for the delay, which can be affected
// by the compiler and optimization settings used.
//
// Note that after a reset the pin is automatically
// configured as GPIO, hence there is no need to configure
// the pin. Some other pins might NOT be configured to GPIO by a reset,
// hence blinking on such a pin requires the additional step of
// configuring the pin as GPIO.
//

// includes the cmsis definitions for the chip
#include "bmptk.h"
			
int main( void ){	

   // must be volatile to prevent optimization of the wait loops
   volatile int i;
   
   // disable the watchdog 
   SIM->COPC = 0;
   
   // enable the clock ti port B
   SIM->SCGC5 |= 0x1 << 10;
   
   // make the pin a GPIO
   PORTB->PCR[ 19 ] = PORT_PCR_MUX( 1 );
   
   // make the pin an output
   PTB->PDDR |= 0x1 << 19;
   
   // loop forever
   while( 1 ){
   
      // make the pin high
      PTB->PSOR = 0x1 << 19;
    
      // wait 
      for( i = 0; i < 200000; i++ ){ 
      }
	
      // make the pin low
      PTB->PCOR = 0x1 << 19;


      // wait 
      for( i = 0; i < 200000; i++ ){
      }
	
	}     
}
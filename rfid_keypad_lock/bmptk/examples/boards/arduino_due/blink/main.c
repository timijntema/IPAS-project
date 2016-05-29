// blink a LED on pin Arduino Due PWM13 = ATSAM3X8E PB27
//
// This Due board has the yellow LED on this pin.
//
// This is a very simple blink-a-LED application.
// It aims to show direct use of the hardware rather than
// good programming practices (like abstracting from the GPIO pin
// used and using a timer for the delay).
//
// It uses the CMSIS header file to access the direction
// and value registers for the pin.
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
			
int main( void ){	

   // must be volatile to prevent optimization of the wait loops
   volatile int i;
   
   // Enable PIO for GPIO: not needed
	 // PIOB->PIO_PER  = ( 1 << 27 );         
   
   // make the pin an output
   PIOB->PIO_OER  = ( 1 << 27 ); 
   
   // loop forever
   while( 1 ){
   
   
      // make the pin high using the
      // Set Output Data Register
      REG_PIOB_SODR = ( 1 << 27 ); 

      // wait 
      for( i = 0; i < 100000; i++ ){ 
      }
	
      // make the pin low using the      
      // Clear Output Data Register
      REG_PIOB_CODR = ( 1 << 27 ); 

      // wait 
      for( i = 0; i <100000; i++ ){ 
      }
	
	}     
	
}
// blink a LED on pin p2
// (the board does not have a LED, you must connect one yourself)
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
   
   // configure the switchmatrix to pass the GPIO out
   LPC_SWM->PINENABLE0 |= ( 0x01 << 3 ); 
    
   // make the pin an output
   LPC_GPIO_PORT->DIR0 = LPC_GPIO_PORT->DIR0 | ( 0x1 << 2 ); 
   
   // loop forever
   while( 1 ){
   
      // make the pin
      LPC_GPIO_PORT->SET0 = ( 0x1 << 2 ); 

      // wait 
      for( i = 0; i < 300000; i++ ){ 
      }
	
      // make the pin
      LPC_GPIO_PORT->CLR0 = ( 0x1 << 2 ); 

      // wait 
      for( i = 0; i < 300000; i++ ){
      }
	
	}     
}
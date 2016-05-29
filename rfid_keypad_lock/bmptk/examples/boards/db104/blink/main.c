// blink one segment
//
// the corresponding segments of the other digits will
// blink faintly due to the pull-ups that are enabled by default.
//
// This is a very simple blink application.
// It aims to show direct use of the hardware rather than
// good programming practices (like abstracting from the GPIO pin
// used and using a timer for the delay).
//
// It uses the CMSIS header file to access the direction
// and value registers for the pins.
// It uses a busy-wait for the delay, which can be affected
// by the compiler and optimization settings used.
//
// Note that after a reset the two pins that are used GPIO1_4 pin are 
// automatically configured as GPIO, hence there is no need to configure
// the pin. Some other pins are NOT configured to GPIO by a reset,
// hence using such a pin requires the additional step of
// configuring the pin as GPIO.
//

// includes the cmsis definitions for the chip
#include "bmptk.h"
			
int main( void ){	

   // must be volatile to prevent optimization of the wait loops
   volatile int i;
    
   // make the pins outputs
   LPC_GPIO0->DIR = LPC_GPIO0->DIR | ( 0x1 << 2 ); 
   LPC_GPIO1->DIR = LPC_GPIO1->DIR | ( 0x1 << 4 ); 
   
   // make one pin low
   LPC_GPIO0->DATA = LPC_GPIO0->DATA & ~ ( 0x1 << 2 );
   
   // loop forever
   while( 1 ){
   
      // make the other pin high
      LPC_GPIO1->DATA = LPC_GPIO1->DATA | ( 0x1 << 4 );

      // wait 
      for( i = 0; i < 300000; i++ ){ 
      }
	
      // make the other pin low
      LPC_GPIO1->DATA = LPC_GPIO1->DATA & ~ ( 0x1 << 4 );

      // wait 
      for( i = 0; i < 300000; i++ ){
      }
	
	}     
}
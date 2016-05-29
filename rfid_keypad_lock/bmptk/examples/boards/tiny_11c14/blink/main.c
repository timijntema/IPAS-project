// blink a LED on pin GPIO2_10
// (= LD1 on the board)
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
      
   // make the pin an output
    LPC_GPIO2->DIR |= ( 0x1 << 10 );
   
   // loop forever
   while( 1 ){
   
      // make the pin high
      LPC_GPIO2->DATA |= ( 0x1 << 10 );

      // wait 
      for( i = 0; i < 300000; i++ ){ 
      }
	
      // make the pin low
      LPC_GPIO2->DATA &= ~ ( 0x1 << 10 );

      // wait 
      for( i = 0; i < 300000; i++ ){
      }
	
	}     
}
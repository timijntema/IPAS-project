// blink a LED on pin pc8 
// (= blue LED on discovery board)
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
   
   // enable port c clock
   RCC->AHBENR |= 1 << 19;
    
   // make pc8 an output
   int temp = GPIOC->MODER;
   temp &= ~(0x03 << (8 * 2));
   temp |=  (0x01  << (8 * 2));
   GPIOC->MODER = temp;
   
   // loop forever
   while( 1 ){
   
      // make pc8 high
      GPIOC->BSRR = 1 << 8;
    
      // wait 
      for( i = 0; i < 100000; i++ ){ 
      }
	
      // make pc8 low
      GPIOC->BSRR = 1 << ( 8 + 16 );


      // wait 
      for( i = 0; i < 100000; i++ ){
      }
	
	}     
}
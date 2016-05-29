// *** this is untested (my board doesn't accept any downloads)
//
// blink the RED part of the three-color LED on pin PIO0_7
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
// Note that after a reset the PIO0_2 pin is automatically
// configured as GPIO, hence there is no need to configure
// the pin. Some other pins are NOT configured to GPIO by a reset,
// hence blinking on such a pin requires the additional step of
// configuring the pin as GPIO.
//

// includes the cmsis definitions for the chip
#include "bmptk.h"
			
int main( void ){	

/*
	// Disable SWDIO on pin 4 (PIO0_2)
	PINENABLE0 |= BIT3;
	// Make bit 2 an output
	GPIO_DIRP0 |= BIT2; 
*/

   // must be volatile to prevent optimization of the wait loops
   volatile int i;
   
   // enable GPIO
   LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);      
   
   // enable switchmatrix
   LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);      
   
   // route function GPIO2 to the pin
   // LPC_SWM->PINENABLE0 |= ( 0x01 << 3 ); 
    
   // make the pin an output
   LPC_GPIO_PORT->DIR0 = LPC_GPIO_PORT->DIR0 | ( 0x1 << 7 ); 
   
   // loop forever
   while( 1 ){
   
      // make the pin high
      LPC_GPIO_PORT->PIN0 |= ( 0x1 << 7 ); 

      // wait 
      for( i = 0; i < 100000; i++ ){ 
      }
	
      // make the pin low
      LPC_GPIO_PORT->PIN0 &= ~ ( 0x1 << 7 ); 

      // wait 
      for( i = 0; i < 100000; i++ ){
      }
	
	}     
}
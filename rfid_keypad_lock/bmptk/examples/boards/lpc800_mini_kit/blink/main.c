// blink the LED on pin PIO0_2
//
// This is a very simple blink-a-LED application.
// It aims to show direct use of the hardware rather than
// good programming practices (like abstracting from the GPIO pin
// used and using a timer for the delay).
//
// It uses the CMSIS header file to configure the pin as GPIO,
// and access the direction and value registers for the pin.
// It uses a busy-wait for the delay, which can be affected
// by the compiler and optimization settings used.
//

// includes the cmsis definitions for the chip
#include "bmptk.h"
			
int main( void ){	

   // must be volatile to prevent optimization of the wait loops
   volatile int i;
   
   // enable GPIO
   LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);      
   
   // enable switchmatrix
   LPC_SYSCON->SYSAHBCLKCTRL |= (1<<7);      
   
   // route function GPIO2 to the pin
   LPC_SWM->PINENABLE0 |= ( 0x01 << 3 ); 
    
   // make the pin an output
   LPC_GPIO_PORT->DIR0 = LPC_GPIO_PORT->DIR0 | ( 0x1 << 2 ); 
   
   // loop forever
   while( 1 ){
   
      // make the pin high
      LPC_GPIO_PORT->PIN0 |= ( 0x1 << 2 ); 

      // wait 
      for( i = 0; i < 300000; i++ ){ 
      }
	
      // make the pin low
      LPC_GPIO_PORT->PIN0 &= ~ ( 0x1 << 2 ); 

      // wait 
      for( i = 0; i < 300000; i++ ){
      }
	
	}     
}
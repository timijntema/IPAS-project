// blink a LED on pin 2
// note: pin 2 is NOT the on-board LED!
// 
// This is the 'blinky' demo, slighly adapted for bmptk
//
// The main() of an ESP8266 application is inside the libraries;
// the user_main() is what you as user can write.
//

// include the definitions for the chip
#include "bmptk.h"  
			
// see eagle_soc.h for these definitions
#define LED_GPIO 2
#define LED_GPIO_MUX PERIPHS_IO_MUX_GPIO2_U
#define LED_GPIO_FUNC FUNC_GPIO2 

// microseconds
#define DELAY 200 * 1000 

extern void ets_wdt_enable (void);
extern void ets_wdt_disable (void);
extern void wdt_feed (void);

void user_init( void ){
	uint8_t state = 0;
	ets_wdt_enable();
	ets_wdt_disable();
	
	// Configure pin as a GPIO
	PIN_FUNC_SELECT( LED_GPIO_MUX, LED_GPIO_FUNC );
	
	for(;;){
		GPIO_OUTPUT_SET( LED_GPIO, state );
		os_delay_us( DELAY );
		state ^= 1;
		wdt_feed();
	}
}

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

// from Ardunet-master

int gpio_pin_register[16] = {PERIPHS_IO_MUX_GPIO0_U,
                             PERIPHS_IO_MUX_U0TXD_U,
                             PERIPHS_IO_MUX_GPIO2_U,
                             PERIPHS_IO_MUX_U0RXD_U,
                             PERIPHS_IO_MUX_GPIO4_U,
                             PERIPHS_IO_MUX_GPIO5_U,
                             PERIPHS_IO_MUX_SD_CLK_U,
                             PERIPHS_IO_MUX_SD_DATA0_U,
                             PERIPHS_IO_MUX_SD_DATA1_U,
                             PERIPHS_IO_MUX_SD_DATA2_U,
                             PERIPHS_IO_MUX_SD_DATA3_U,
                             PERIPHS_IO_MUX_SD_CMD_U,
                             PERIPHS_IO_MUX_MTDI_U,
                             PERIPHS_IO_MUX_MTCK_U,
                             PERIPHS_IO_MUX_MTMS_U,
                             PERIPHS_IO_MUX_MTDO_U};

// #define GPIO_PIN_ADDR(i)    (GPIO_PIN0_ADDRESS + i*4)

void pin_configure_as_output( int pin ){
    int mode = 0;
    if ((0x1 << pin) & 0b110101) {
        PIN_FUNC_SELECT(gpio_pin_register[pin], 0);
    } else {
        PIN_FUNC_SELECT(gpio_pin_register[pin], 3);
    }
    
    PIN_PULLDWN_DIS(gpio_pin_register[pin]);
    PIN_PULLUP_DIS(gpio_pin_register[pin]);
    
    if (mode) {
        GPIO_REG_WRITE(GPIO_ENABLE_W1TC_ADDRESS, 1<<pin); // GPIO input
    } else {
        GPIO_REG_WRITE(GPIO_ENABLE_W1TS_ADDRESS, 1<<pin); // GPIO output
    }
}

void pin_set( int pin, char state ){
    if (state) {
        GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, 1<<pin); // GPIO high
    } else {
        GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, 1<<pin); // GPIO low
    }
}
			
// see eagle_soc.h for these definitions
#define LED_GPIO 2
#define LED_GPIO_MUX PERIPHS_IO_MUX_GPIO2_U
// #define LED_GPIO_FUNC FUNC_GPIO2 
#define LED_GPIO_FUNC 0

// microseconds
#define DELAY 800 * 1000 

extern void ets_wdt_enable (void);
extern void ets_wdt_disable (void);
extern void wdt_feed (void);

int led1 = 12;
int led2 = 13;
int led3 = 5;

void pulse( int n ){
		pin_set( n, 1 );
		os_delay_us( 200 * 1000 );
		pin_set( n, 0 );
		os_delay_us( DELAY );
}

void user_init( void ){
	ets_wdt_enable();
	ets_wdt_disable();
	
	pin_configure_as_output( led1 );
	pin_configure_as_output( led2 );
	pin_configure_as_output( led3 );
	pin_configure_as_output( 2 );
	
	for(;;){
		pulse( 2 );
		// wdt_feed();
	}
}

// implementation of the uart functions
//

#include "bmptk.h"
#include "sw-uart.h"
#include "timer.h"
#include "limits.h"

#define BIT_TIME ( (1000 * 1000) / (BMPTK_BAUDRATE) )

void txd_low( void ){
   LPC_GPIO1->DATA = LPC_GPIO1->DATA & ~ ( 1 << 7 );
}

void txd_high( void ){
   LPC_GPIO1->DATA = LPC_GPIO1->DATA | ( 1 << 7 );
}

void uart_init( void ){
   LPC_GPIO1->DIR = LPC_GPIO1->DIR | ( 1 << 7 ); // make GP1_7 output
   txd_high();
}

void uart_put_char( char c ){
   unsigned int t = now();
   int i;
   
   // start bit
   txd_low();
   t += BIT_TIME;
   await( t );
   
   for( i = 0; i < 8; i++ ){
      // one data bit
      if( ( c & 0x01 ) == 0 ){ 
          txd_low(); 
      } else { 
          txd_high(); 
      }   
      c = c >> 1;
      t += BIT_TIME;
      await( t );       
   }
   
   // stop bits
   txd_high();
   t += 2 * BIT_TIME;
   await( t );   
}

void uart_put_string( const char *s ){
   while( *s != '\0' ){
      uart_put_char( *s );
      s++;
   }      
}

void uart_put_int_decimal( int x ){
   int xx;
   
   if( x == INT_MIN ){
   
     uart_put_string( "-2147483648" ); 
   
   } else { 
   
      if( x < 0 ){
   
         uart_put_char( '-' );
         uart_put_int_decimal( -x );
      
      } else {
   
         xx = x / 10;
         if( xx != 0 ){
            uart_put_int_decimal( xx );
         }
         uart_put_char( '0' + x % 10 );
         
      }   
   }   
}

void uart_put_int_hexadecimal_digit( int x ){
   x = x & 0x0F;
   if( x < 10 ){
      uart_put_char( x + '0' );
   } else {
      uart_put_char( ( x + 'A' ) - 10 );
   }      
}

void uart_put_int_hexadecimal( unsigned int x, int n ){
   while( n > 0 ){
      uart_put_int_hexadecimal_digit( x >> (( n - 1 ) * 4 ) );
      n--;
   }
}



		

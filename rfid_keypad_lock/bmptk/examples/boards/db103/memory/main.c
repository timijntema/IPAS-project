// This example shows the memory map used on an LPC1114
// and the working of the bmptk implementation of malloc()       
                                                      
#include <stdlib.h>
#include "bmptk.h"
#include "timer.h"
#include "sw-uart.h"

#ifndef BMPTK_TARGET_db103
   #error "this application is for a DB103" 
#endif 

void print_memmap_line( const char *s, char *p, char dec ){ 
   uart_put_string( s );
   uart_put_int_hexadecimal( (int) p, 8 );
   if( dec ){
      uart_put_string( " (" );
      uart_put_int_decimal( (int) p ); 
      uart_put_string( ") " );
   }
   uart_put_string( "\n" );
}

int total = 0;
void print_allocate( int n ){
   uart_put_string( "   allocate " );
   uart_put_int_decimal( n );
      
   void *p = malloc( n );

   uart_put_string( " p=" );
   uart_put_int_hexadecimal( (int) p, 8 );
      
   if( p == 0 ){
      uart_put_string( " failed" );
   } else {
      total += n;
      uart_put_string( " OK total = " );
      uart_put_int_decimal( total );   
   }   
   uart_put_string( "\n" );
}

// provided by the linkerscript
  
extern char __rom_start;
extern char __rom_end;
extern char __rom_size;

extern char __ram_start;
extern char __ram_end;
extern char __ram_size;

extern char __text_start;
extern char __text_end;
extern char __text_size;

extern char __data_init_start;
extern char __data_init_end;
extern char __data_init_size;

extern char __data_start;
extern char __data_end;
extern char __data_size;

extern char __bss_start;
extern char __bss_end;
extern char __bss_size;

extern char __stack_start;
extern char __stack_end;
extern char __stack_size;

extern char __heap_start;
extern char __heap_end;
extern char __heap_size;

const char text_in_code[] = "this text is in the CODE segment\n";
char       text_in_data[] = "text in the DATA segment\n";

void memmap_print(){
   uart_put_string( text_in_code );
   uart_put_string( text_in_data ); 
   uart_put_string( "\n" );

   uart_put_string( "Chip\n" ); 
   print_memmap_line( "   ROM start         : ", &__rom_start, 0 );
   print_memmap_line( "   ROM end           : ", &__rom_end, 0 );
   print_memmap_line( "   ROM size          : ", &__rom_size, 1 );
   uart_put_string( "\n" );
   print_memmap_line( "   RAM start         : ", &__ram_start, 0 );
   print_memmap_line( "   RAM end           : ", &__ram_end, 0 );
   print_memmap_line( "   RAM size          : ", &__ram_size, 1 );
   uart_put_string( "\n" );  
   
   uart_put_string( "Application\n" );
   print_memmap_line( "   TEXT start        : ", &__text_start, 0 );
   print_memmap_line( "   TEXT end          : ", &__text_end, 0 );
   print_memmap_line( "   TEXT size         : ", &__text_size, 1 );
   uart_put_string( "\n" );
   print_memmap_line( "   DATA (ROM) start  : ", &__data_init_start, 0 );
   print_memmap_line( "   DATA (ROM) end    : ", &__data_init_end, 0 );
   print_memmap_line( "   DATA (ROM) size   : ", &__data_init_size, 1 );
   uart_put_string( "\n" );
   print_memmap_line( "   DATA (RAM) start  : ", &__data_start, 0 );
   print_memmap_line( "   DATA (RAM) end    : ", &__data_end, 0 );
   print_memmap_line( "   DATA (RAM) size   : ", &__data_size, 1 );
   uart_put_string( "\n" );
   print_memmap_line( "   BSS start         : ", &__bss_start, 0 );
   print_memmap_line( "   BSS end           : ", &__bss_end, 0 );
   print_memmap_line( "   BSS size          : ", &__bss_size, 1 );
   uart_put_string( "\n" );
   print_memmap_line( "   STACK bottom      : ", &__stack_start, 0 );
   print_memmap_line( "   STACK top         : ", &__stack_end, 0 );
   print_memmap_line( "   STACK size        : ", &__stack_size, 1 );
   uart_put_string( "\n" );
   print_memmap_line( "   HEAP bottom       : ", &__heap_start, 0 );
   print_memmap_line( "   HEAP top          : ", &__heap_end, 0 );
   print_memmap_line( "   HEAP size         : ", &__heap_size, 1 );
   uart_put_string( "\n" );
   
   print_allocate( (int)&__heap_size - 9 );
   print_allocate( 3 );
   print_allocate( 1 );
   print_allocate( 1 );
}   

char title[] = "memory map test\n";
char s[ 67 ];
		
int main( void ){	
   
   timer_init();
   uart_init();
   uart_put_string( title );
   s[ 0 ] = '\n'; s[ 1 ] = '\0';
   uart_put_string( s );   
   memmap_print();
   uart_put_string( "\ndone\n" );
   
   void *p = malloc( 10 );
   // just to make sure p is not optimized away
   if( p == NULL ){ uart_put_string( "NULL" ); }
   
   // un-commenting the next line will cause a linker error
   // free( p );
   
   return 0;
}
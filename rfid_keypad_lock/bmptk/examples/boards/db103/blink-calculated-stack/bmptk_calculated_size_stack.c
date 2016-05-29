// This file supplies the BMPTK stack space variable.
//
// The size is specified by the 24 command line argument.

// NATIVE means do not create a stack, the native stack is used.
// supply a dummy size to make the compilation successful.
#define NATIVE 8

// AUTOMATIC means calculate the required stack size and allocate a stack 
// of that size, but the application must first be linked, and for that 
// a dummy stack must be used to make the compilation successful.
#define AUTOMATIC 8

unsigned char bmptk_stack[ 24 ] 
   __attribute__ (( section( ".bmptk_stack" )));
file: bmptk/targets/README.txt

Bare Metal Programming Tool Kit targets directory

This directory contains subdirectories for the supported targets,
which (can) contain
- startup code
- linkerscrip (either readdy-to-use or a template)
- chip headers

directories
   armv7       : older ARM chips
   avr         : Atmel AVR 8-bit microcontrollers
   cortex      : ARM Cortex microcontrollers
   esp8266     : the famous microcontroller-with-Wifi chip
   msp430      : TI msp430 16-bit microcontrollers
   win         : windows native
   
notes   
   msp430/include/iomacros.h   
      __interrupt(vec) commented out because it conflicts 
      with the msp430-gee on fedora


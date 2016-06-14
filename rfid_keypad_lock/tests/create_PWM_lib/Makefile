#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
# 
#############################################################################

# source files in this project (main.cpp is automatically assumed)
SOURCES := matrixKeypad.cpp passwordOperations.cpp PWM_signal.cpp servo.cpp mfrc522v2.cpp

# header files in this project
HEADERS := matrixKeypad.hpp passwordOperations.hpp PWM_signal.hpp servo.hpp mfrc522v2.hpp

# other places to look for files for this project
SEARCH  :=  D:/programmeren/Projecten/IPAS-project/rfid_keypad_lock/Finished_librarys

# set BMPTK to the next higher directory and defer to the Makefile.due
BMPTK := $(BMPTK)../
include $(BMPTK)Makefile.due

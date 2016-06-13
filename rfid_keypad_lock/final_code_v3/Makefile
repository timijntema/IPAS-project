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
SOURCES := matrixKeypad.cpp accessOperations.cpp PWM_signal.cpp servo.cpp mfrc522.cpp RFID.cpp

# header files in this project
HEADERS := matrixKeypad.hpp accessOperations.hpp PWM_signal.hpp servo.hpp mfrc522.hpp RFID.hpp

# other places to look for files for this project   D:/programmeren/Projecten/IPAS-project/rfid_keypad_lock/Finished_librarys
SEARCH  :=  C:\Users\Tim\Documents\GitHub\IPAS-project\rfid_keypad_lock\Finished_librarys

# set BMPTK to the next higher directory and defer to the Makefile.due
BMPTK := $(BMPTK)../
include $(BMPTK)Makefile.due

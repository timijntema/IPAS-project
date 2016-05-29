#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2015
#
# This file is in the public domain.
# 
#############################################################################

# these directories contain ESP8266 examples
TARGET     ?= esp8266

# set BMPTK to the next higher directory
BMPTK := $(BMPTK)../

# Defer to the next higher Makefile.inc to do the work
include $(BMPTK)Makefile.inc

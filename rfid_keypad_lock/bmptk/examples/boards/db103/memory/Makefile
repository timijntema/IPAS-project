#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2015
#
# This file is in the public domain.
# 
#############################################################################

HEADER   += timer.c sw-uart.h
SOURCES  += timer.c sw-uart.c
HEAP     := BMPTK

# Defer to the next higher Makefile.inc to do the work
BMPTK := $(BMPTK)../
include $(BMPTK)Makefile.inc
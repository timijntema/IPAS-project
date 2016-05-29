#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2015
#
# This file is in the public domain.
# 
#############################################################################

# Specify target chip or board or system
TARGET     := db103

# set BMPTK to the next higher directory
BMPTK := $(BMPTK)../

# Defer to the next higher Makefile.inc to do the work
include $(BMPTK)Makefile.inc

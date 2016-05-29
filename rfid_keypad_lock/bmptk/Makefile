#############################################################################
# 
# Makefile for bmptk zip file, etc.
#
# Copyright (c) 2012 .. 2015 Wouter van Ooijen (www.voti.nl)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)
#
# This file makes some meta-stuff that is probably 
# interesting only for me as bmptk maintainer.
#
# targets:
#    clean                  : total cleanup 
#    build, examples        : build all examples
#    commit                 : clean, git commit -a, git push 
#    zip                    : create bmptk.zip, includes documentation
#    www                    : create webpages (www subdir, includes zip)
#    ftp                    : create webpages and ftp to website
#                             (won't work from your PC ;) )
#    v1th04, v2cpse2        : creates .zip for a specific course
#
# The commit and ftp targets won't work on your PC because they require
# tools and/or passwords that are outside the bmptk tree.
# 
#############################################################################

ifeq ($(OS),Windows_NT)
   # WPF is the windows prefix for 32-bit applications
   ifeq ($(wildcard C:/Program\ Files\ (x86)/* ),)
      WPF := C:/Program Files
   else
      WPF := C:/Program Files (x86)
   endif
endif

ifneq ($(wildcard Makefile.custom),)
   include Makefile.custom
else   
   include Makefile.local
endif

.PHONY: clean doc docs documentation build all examples 
.PHONY: zip commit tests webpage
.PHONY: v1th04 v2cpse2

all: examples 
build: examples 

examples:
	cd examples && make build
	@echo "**** examples build completed succesfully"

tests:
	cd tests && make build
	@echo "**** tests build completed succesfully"

zip: clean	 
	$(7za) a -tzip bmptk.zip .
	@echo "**** zip build completed succesfully"
   
webpage:
	bmptk-rm -rf transfer
	mkdir transfer
	-cp ../_ftp_crc_s transfer
	cp webpage/* transfer
	python \ftpbmptk.py
	cp transfer/_ftp_crc_s ..
	@echo "**** ftp transfer completed succesfully"
   
clean:
	bmptk-rm -rf index.html docs bmptk.zip bmptk-th04.zip transfer
	cd examples && make clean		
	@echo "**** cleanup completed succesfully"
		
commit: 
	make clean
	git commit -a -m 'work'
	git push

ftp: zip
	bmptk-rm -rf transfer
	mkdir transfer
	-cp ../_ftp_crc_s transfer
	cp bmptk.zip transfer
	python \ftpbmptk.py
	cp transfer/_ftp_crc_s ..
	@echo "**** ftp transfer completed succesfully"
      
TH04LIST += bmptk.docx bmptk.h README.txt
TH04LIST += license.txt license_1_0.txt
TH04LIST += Makefile Makefile.inc Makefile.local Makefil.subdirs
TH04LIST += targets
TH04LIST += tools
TH04LIST += hwcpp
TH04LIST += examples/courses/v1th04

v1th04: clean doc
	$(7za) a -tzip bmptk-v1th04.zip $(TH04LIST)
	@echo "**** bmptk-v1th04.zip built succesfully"   
		
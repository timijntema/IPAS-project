file: bmptk/README.txt

Copyright (c) 2012 .. 2105 Wouter van Ooijen (wouter@voti.nl)

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at 
http://www.boost.org/LICENSE_1_0.txt)

This is the bmptk (Bare Metal Programming Tool Kit) main directory.
Bmptk is a minimalist make-based development environment for 
small micro-controllers using GCC C, C++ or assembler on windows
(with limited support for Linux).

Bmptk contains hwcpp, a C++ library for programming small micro-controllers.
Bmptk can be used without hwcpp, and with some effort hwcpp could be
used without bmptk.

This directory contains :

   files
      - bmptk.docx         : bmptk manual, ms-word format
      - bmptk.h            : top-level header file for bmptk
      - license.txt        : license (== boost license)
      - license_1_0.txt    : boost license
	   - Makefile           : makes examples etc.
      - Makefile.local     : locations of the external tools (edit as needed)
      - Makefile.template  : template for a bmptk project(s) makefile
      - Makefile.inc       : included by the project makefile
      - Makefile.subdirs   : included by a makfile to recurse subdirectories
      - notes.txt          : notes and reminders for me
      - README.txt         : this file

   subdirectories
      - attic              : resting place for old stuff
      - examples           : examples directories
      - hwcpp              : the hwcpp library
      - targets            : chip and board specific stuff
      - tools              : tools (executables, scripts)

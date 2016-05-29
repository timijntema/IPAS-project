# ===========================================================================
# 
# BMPTK's Stack Size Calculator:
# calculate the stack use from an .lss file
#
# (c) Wouter van Ooijen (wouter@voti.nl)
# 2015-12-18 version 0.1 work-in-progress
# license: Boost Software License - Version 1.0
#
# ===========================================================================
#
# Known limitations
# 
# tested only on Windows
# correct lss format is assumed, not (always) checked
# hand-written asm code often causes trouble
# only a top-level unused subroutine is reported as 'never called'
# only a few simple sp-changing sequences are supported 
#   (and not fully checked)
#
# ===========================================================================
#
# ToDo list
#
# warn bij in-balance
# what to do with a routine that just jumps to iself? like __aeabi_idiv0
# test recursion, virtuals
# avr __bad_interrupt chained to main??
#
# ===========================================================================

from __future__ import print_function
import operator, subprocess, sys, os, argparse, threading

def demangle( filt, name ):
   "de-mangle a C++ functionn name using the c++filt tool"
   if filt:
      return subprocess.check_output( [ filt , name] ).decode("utf-8").strip()
   else: 
      return name   
   
def file_from_text( file_name, text ):
   "write the string text to the file_name"
   f = open( file_name, "w" )
   f.write( text )
   f.close()   

def text_from_file( file_name ):
   "return the text in the file"
   f = open( file_name, "r" )
   result = f.read()
   f.close()
   return result
   
def replace_extension( file_name, extension ):
   "replace the extension of file_name as indicated"
   return os.path.splitext( file_name )[0] + '.' + extension
   
def signed( x ):
   "return a signed value from an unsigned 32-bit 'bit pattern'"
   if x >= 2**31:
      x = x - 2**32
   return x
   
def remove( s, x ):
   "return s with all ocurrences of a char that appears in x removed"
   for c in x:
      s = s.replace( c, "" )
   return s      
   
def replace( s, x, y ):
   "return s with all ocurrences of a char that appears in x replaced by y"
   for c in x:
      s = s.replace( c, y )
   return s   
   
def label( line ):
   "return the text enclosed in <> brackets"
   return ( line + '<' ).split( '<' )[ 1 ].split( '>' )[ 0 ]   
   
def stripquotes( line ):
   "remove surrounding quotes from a line"
   line = line.strip()
   if( line + " " )[ 0 ] in "\"'":
      return line.strip( line[ 0 ] )
   else:
      return line   
      
# ===========================================================================

class problem:
   "a problem that can be related to a specific .lss line"
   
   def __init__( self, line_nr, text, fatal = 1 ):
      self.line_nr = line_nr
      self.text = "[%4d] %s" % ( line_nr, text )
      self.fatal = fatal
      
   def print( self, stop = 1 ):
      if self.fatal:
         print( "FATAL   " + self.text, file = sys.stderr )
         if stop: 
            exit( -1 )
      else:
         print( "WARNING " + self.text, file = sys.stderr )

# ===========================================================================

class assembler_line:
   "an lss assembler line (not being an empty line, label, etc.)"
   
   def __init__( self, cpu, line, line_nr, subroutine ):
      self.cpu = cpu
      self.line = line
      self.line_nr = line_nr
      self.subroutine = subroutine
      
      self.call = 0
      self.extra = 0
      self.is_call = 0
      self.is_data = 0
      self.my_problems = []
      self.target = None
      
      # de-code the line a bit
      # address           opcode  arguments  
      #   20:	22fa      	movs	r2, #250	; 0xfa
      if 0: print( line )
      line = line.strip() + "\t\t"
      self.address = line.split( ":" )[ 0 ]
      self.opcode = line.split( "\t" )[ 2 ]  
      self.arguments = line.split( "\t", 3 )[ 3 ]
      self.arguments = remove( self.arguments, "{} ;([" ).rstrip( '\t' )
      self.arguments = replace( self.arguments, "\t<", "," ).split( "," )

      #  67d:	74206948 65726568 7c000000                       Hi there...
      if ( line.count( "\t" ) == 3 ) or ( self.opcode.startswith( "." )):
         self.is_instruction = 0
         if self.opcode == ".word":
            # address           opcode  arguments  
            #   4:	00000051 	.word	0x00000051
            self.is_data = 1
            self.value = signed( int( self.arguments[ 0 ], 0 ) )
         return
         
      self.is_instruction = 1
      
   def problem( self, s, fatal = 1 ):
      self.my_problems.append( 
         problem( 
            self.line_nr, "[%s] %s" % 
               ( self.subroutine.name_demangled, s ), 
            fatal ) )   
            
   def parse( self, level ):
      self.level = level
      self.delta = 0
      
      if not self.is_instruction:
         return    
         
      # save register values at entry   
      self.old_registers = self.subroutine.registers.copy()
                
      if self.cpu == "msp430":
         self.parse_msp430()
      elif self.cpu == "avr":
         self.parse_avr()
      elif self.cpu == "cortex":
         self.parse_cortex();
      else:
         print( "unknown cpu [%s]" % self.cpu )
         exit( -1 )      
         
      self.level += self.delta      

   def parse_msp430( self ):
      # invalidate any value stored for the destination register
      if len( self.arguments ) > 0: 
         target = self.arguments[ 0 ]
         if target in self.subroutine.registers:
            del self.subroutine.registers[ target ]              
            
      return      
      
   def parse_avr( self ):
      # invalidate any value stored for the destination register
      if len( self.arguments ) > 0: 
         target = self.arguments[ 0 ]
         if target in self.subroutine.registers:
            del self.subroutine.registers[ target ]              
            
      value = ( self.line + ';' ).split( ';' )[ 1 ]      
   
      if self.opcode == "push":
         self.delta = +1
         
      elif self.opcode == "pop":
         self.delta = -1
         
      elif (
         ( self.opcode == "rcall" )
         and ( self.arguments[ 0 ] == ".+0" )
      ):
         # rcall	.+0
         self.delta = +2
         
      elif self.opcode == "call":  
         # call	0x96	; 0x96 <main>      
         self.target = self.line.split( '<' )[ 1 ].split( '>' )[ 0 ]
         self.extra = 2
         self.is_call = 1
         
      elif self.opcode == "in":
         # in	r28, 0x3d	; 61
         if int( value ) == 61:
            # pretend that the target is a 16-bit register
            self.subroutine.registers[ target ] = - self.level
         
      elif self.opcode == "sbiw":
         # sbiw	r28, 0x0e	; 14
         if target in self.old_registers:
            # pretend that the register is 16 bits
            self.subroutine.registers[ target ] = \
               self.old_registers[ target ] - int( value )
               
      elif self.opcode == "adiw":
         # adiw	r28, 0x0e	; 14
         print( self.line )
         print( self.old_registers )
         if target in self.old_registers:
            # pretend that the register is 16 bits
            print( self.old_registers[ target ] + int( value ) )
            self.subroutine.registers[ target ] = \
               self.old_registers[ target ] + int( value )   
               
      elif self.opcode == "out":         
         # out	0x3d, r28	; 61
         if int( value ) == 61:
             source = self.arguments[ 1 ]
             if source in self.old_registers:
                # stack grows down
                self.delta = ( - self.old_registers[ source ] ) - self.level
             else:
                if self.subroutine.name != "__vectors":
                   # stack is set to its initial value in __vectors
                   self.my_problems.append( problem( 
                     self.line_nr, "register %s value unknown" %  source ) )                
   
      return   
      
   def parse_cortex( self ):
      "second pass: parse() must be called once "
      "for each instruction, in order"
      
      self.opcode = self.opcode \
         .replace( "add.w", "addw" ) \
         .replace( "sub.w", "subw" )
      
      # invalidate the value stored in the target register   
      if self.opcode != "str":
         target = self.arguments[ 0 ]
         if target in self.subroutine.registers:
            del self.subroutine.registers[ target ]

      if self.opcode == "bl":
         self.target = self.line.split( '<' )[ 1 ].split( '>' )[ 0 ]
         self.is_call = 1
         
      if self.opcode == "b.n":
         self.target = self.line.split( '<' )[ 1 ].split( '>' )[ 0 ]
         if self.target.find( "+" ) == -1:
            self.is_call = 1
         
      elif self.opcode == "blx":
         reg = self.arguments[ 0 ]
         d = self.old_registers.get( reg, None )
         if ( d != None ) and ( d == 0x1FFF1FF1 ):
            # call to ROM IAP, ignore
            self.problem( "IAP call, assumed to use no stack space", 0 )
         else:   
            self.problem( "indirect call" )
         
      elif self.opcode.startswith( "mov" ):
         # movs	r3, #250	; 0xfa
         if self.arguments[ 1 ].startswith( "#" ):
            self.subroutine.registers[ self.arguments[ 0 ] ] = \
               int( self.arguments[ 1 ][ 1: ] )
               
      elif self.opcode.startswith( "lsl" ):    
         # lsls	r3, r3, #4
         if self.arguments[ 1 ].startswith( "r" ) \
               and ( len( self.arguments ) > 2 ) \
               and self.arguments[ 1 ] in self.old_registers \
               and self.arguments[ 2 ].startswith( "#" ):
            self.subroutine.registers[ self.arguments[ 0 ] ] = \
               self.old_registers[ self.arguments[ 1 ] ] * \
               ( 2 ** int( self.arguments[ 2 ][ 1 : ] ) )
         
      elif self.opcode == "push":
         # push	{r7, lr}
         self.delta = 4 * len( self.arguments )     
         
      elif self.opcode == "pop":
         # push	{r7, pc}
         self.delta = - 4 * len( self.arguments )  
         
      elif self.opcode == "ldr" \
            and ( self.arguments[ 1 ] == "pc" ):
         # ldr	r7, [pc, #24]	; (40 <_Z4keepv+0x20>)         
         self.subroutine.registers[ self.arguments[ 0 ] ] = \
            self.subroutine.data[ int( self.arguments[ 3 ], 16 ) ]
         
      elif ( self.opcode in ( "addw", "subw" ) ) \
            and ( len( self.arguments ) >= 3 ) \
            and ( self.arguments[ 0 ] == "sp" ) \
            and ( self.arguments[ 1 ] == "sp" ):

         if self.arguments[ 2 ].startswith( "#" ):
            # subw	sp, sp, #1756	; 0x6dc
            self.delta = int( self.arguments[ 2 ][ 1: ] )
            
         if self.opcode == "addw":
            # stack grows down: add decreases stack use
            self.delta = - self.delta               

      elif ( self.opcode in ( "add", "sub" ) ) \
            and ( len( self.arguments ) >= 2 ) \
            and ( self.arguments[ 0 ] == "sp" ):
            
         if self.arguments[ 1 ].startswith( "#" ):
            # sub	sp, #404	; 0x194
            self.delta = int( self.arguments[ 1 ][ 1: ] )
            
         elif self.arguments[ 1 ].startswith( "r" ):   
            # add	sp, r3
            reg = self.arguments[ 1 ]
            d = self.old_registers.get( reg, None )
            if d == None:
               self.my_problems.append( problem( 
                  self.line_nr, "register %s value unknown" % reg ) )
            else:   
               self.delta = d
               
         else:
            self.my_problems.append( problem( 
               self.line_nr, "format not recognized" ) )                     
            
         if self.opcode == "add":
            # stack grows down: add decreases stack use
            self.delta = - self.delta         
         
      elif self.arguments[ : 1 ].count( "sp" ):
         self.my_problems.append( problem( self.line_nr, "sp as argument" ) )         
         
      else:
         pass 
         
   def __str__( self ):
      return "[%6d:%6d:%6d:%6d:%6d] %s" % ( 
         self.delta, self.level, self.extra, 
         self.call, self.total, self.line 
      )
         
# ===========================================================================

class subroutine:
   "a single assembler subroutine"
   
   # stack administration for each instruction:
   #   delta = change in stack use caused by the instruction
   #           eg: push = +1 (avr) or +4 (cortex)
   #   base  = stack use at the start of the instruction
   #           = running total of delta's
   #   extra = temporary stack during execution of the instruction itself
   #           eg: call = +2 (avr), but bl = 0 (cortex)
   #   call  = stack use of the routine called by this instruction
   #   peak  = peak stack use during execution of the instruction 
   #           and/or the called subroutine
   # The stack use of a subroutine is the highest peak of all its instructions.
 
   def __init__( self, cpu, subroutines, filt ):
      self.name = None
      self.name_demangled = None
      self.calc_stack_use = None
      self.line_nr = -1
      self.lines = []
      self.my_problems = []
      self.cpu = cpu
      self.subroutines = subroutines
      self.data = {}
      self.registers = {}
      self.instructions = 0
      self.called = 0
      self.is_root = 0       
      self.filt = filt      
      self.level = 0
      
   def add( self, line, line_nr ):
      "add a single lss line to the subroutine"
      if( line[ 0 ] != " " ):
         # subroutine label?
         if self.name == None:
            self.line_nr = line_nr
            self.name = line.split( '<' )[ 1 ].split( '>' )[ 0 ]
            self.name_demangled = demangle( self.filt, self.name )
      else:
         decoded_line = assembler_line( self.cpu, line, line_nr, self )
         self.lines.append( decoded_line )
         self.instructions += decoded_line.is_instruction
         if decoded_line.is_data:
            self.data[ int( decoded_line.address, 16 ) ] = int( decoded_line.value )  
         
   def parse( self ):
      "is called once (by stack_use) to parse the subroutine"
      level = 0
      for line in self.lines:   
         line.parse( level )
         level = line.level
         self.my_problems.extend( line.my_problems )         
         
   def stack_use( self ):         
      if self.calc_stack_use == None:
         self.parse()
         self.calc_stack_use = 0
         for line in self.lines:
            line.call = 0
            if line.is_call:
               if line.target in self.subroutines:
                  target = self.subroutines[ line.target ]
                  target.called += 1
                  line.call = target.stack_use()
               else:
                  self.my_problems.append( 
                     problem( self.line_nr, 
                        "target [%s] unknown" % line.target ))               
                  
            line.total = line.level + line.extra + line.call
            self.calc_stack_use = max ( self.calc_stack_use, line.total )
      return self.calc_stack_use   
      
   def __str__( self ):
      all = ''
      all += "[%4d] : %s\n" % ( self.line_nr, self.name_demangled )
      all += "   %s\n"      % self.name
      all += "   called: %d   stack use: %d\n" % ( self.called, self.stack_use() )
      all += "     delta   base  extra   call   peak\n"
      for line in self.lines:
         all += "   %s\n" % line    
      return all
      
# ===========================================================================

class application:
   "the appliation inf as read from a .lss file"
   
   def __init__( self, cpu, file_name, root_names, filt ):      
      self.cpu = cpu
      self.root_names = root_names
      self.my_problems = []
      self.subroutines = {}
      
      if self.cpu == "msp430"   :
         self.non_breaking_labels = [
            "__crt0_init_bss",
            "__crt0_call_init_then_main",
            "_msp430_run_init_array",
            "_msp430_run_preinit_array",
            "_msp430_run_fini_array",
            "_msp430_run_array",
            "_msp430_run_done",
            "register_tm_clones",
            "__do_global_dtors_aux",
            
         ]
         self.known_not_called = [
         ]
         
      elif self.cpu == "avr"   :
         self.non_breaking_labels = [
            "__ctors_end",
            "__do_copy_data",
            "__do_clear_bss",
            ".do_clear_bss_loop",
            ".do_clear_bss_start",
         ]
         self.known_not_called = [
            "__bad_interrupt",
            "_exit",
            "__stop_program",
         ]
         
      elif self.cpu == "cortex":
         self.non_breaking_labels = [
         ]
         self.known_not_called = [
         ]
         
      else:   
         print( "unknown cpu [%s]" % self.cpu )
         exit( -1 )           
      
      f = open( file_name, "r" )
      next = subroutine( cpu, self.subroutines, filt )
      line_nr = 0
      for line in f.readlines():
         line = line.replace( "\n", '' )
         line_nr += 1
         # print( "[%s]" % line )
         if line == "":
            pass
         elif ( line[ 0 ] == "0" ) \
         and not ( label( line ) in self.non_breaking_labels ) \
         and not self.msp430_ignore( label( line )) \
         and next.name != None :
            self.subroutines[ next.name ] = next
            next = subroutine( cpu, self.subroutines, filt )
            next.add( line, line_nr )
         elif( line[ 0 ] in " 0" ):   
            next.add( line, line_nr )
         else:
            pass      
      self.subroutines[ next.name ] = next
      for name in self.root_names:
         self.mark_as_root( name )
         
   def msp430_ignore( self, label ):
      return (
         ( label[ 0 ] == '.' )
         or ( label == "L0\1" ))

   def mark_as_root( self, name ):
      if name in self.subroutines:
         self.subroutines[ name ].called += 1
         self.subroutines[ name ].is_root = 1
      else:   
         self.my_problems.append( 
            problem( 0, "root [%s] not found" % name, 1 ))
         
   def problems( self ):
      if self.my_problems == []:
         for name, subroutine in sorted( self.subroutines.items() ):
            if ( subroutine.level != 0 ):
               problem( subroutine.line_nr, 
                   "[%s] has a non-0 stack delta" % subroutine.name_demangled, 0 )
            if ( subroutine.called == 0 ) \
                  and ( subroutine.instructions > 0 ) \
                  and not ( subroutine.name in self.known_not_called ):
               self.my_problems.append( 
                  problem( subroutine.line_nr, 
                     "[%s] is never called" % subroutine.name_demangled, 0 ))
            self.my_problems.extend( subroutine.my_problems )         
      return self.my_problems           

   def stack_use( self, name ):      
      if name in self.subroutines:
         return self.subroutines[ name ].stack_use()   
      else:
         self.my_problems.append( problem( 0, 
            ( "root [%s] not found" % name ), 1 ))               
         return 0
      
   def expanded_call_tree_view( self, name, prefix = "", step = "   " ):  
      all = ""
      all += prefix + name
      for call in subroutines[ name ].calls:
         all += expanded_call_tree_view( 
            self, call.target, prefix + step, step )   
      return all
      
   def annotated_view( self ):
      all = ""
      for name in self.root_names:
         all += "[%s] uses %d stack bytes\n" % ( name, self.stack_use( name ) )
      all += "\n"
      for name, subroutine in sorted( self.subroutines.items() ):
         all += str( subroutine ) + "\n"
      return all
      
   def subroutines_list( self ):
      all = []
      for name, subroutine in sorted( subroutines.items() ):
         all.append( name )
      return all               
      
   def handle_problems( self, abort = 1 ):
      px = self.problems()   
      fatals = 0
      if len( px ):
         for p in px:
            p.print( 0 );
            if p.fatal:
               fatals += 1
      if fatals and abort:
         print( "%d fatal problems found\n" % fatals )
         exit( -1 )

# =========================================================================== 

def arguments_parser():
   parser = argparse.ArgumentParser( 
      description = \
      'Calculate required stack size from an lss file.'
      'By default, a file bmptk_culated_size_stack.c file is created '
      'that contains one line:\n'
      '   unsigned char bmptk_stack[ 40 ] '
         '\__attribute__ ((section(".bmptk_stack")));'
   )   

   parser.add_argument(
      'cpu', 
      #type = string, 
      help = 'target cpu: cortex, avr, msp430' )

   parser.add_argument(
      'root', 
      help = 'root function for which the stack size is calculated' )

   parser.add_argument(
      'input_file', 
      help = '.lss input file' )


   parser.add_argument(
      'template', 
      help = 'The file that is copied to the generated stack output_file. '
      'Any ocurrence of STACK_SIZE is replaced by the calculated stack size.' )   
      
   parser.add_argument(
      'output_file', 
      help = 'The name of the file to which the output (copy of the template) is written' )
      
   parser.add_argument(
      '-annotate', 
      help = 'create annotated version of the lss file' )
      
   parser.add_argument(
      '-filt', 
      help = 'FILT executable to use to demangle C++ names' )
 
   parser.add_argument(
      '-size', 
      type = int, 
      help = 'SIZE overrides the calculated stack size' )

   parser.add_argument(
      '-delta', 
      type = int, 
      default = 0,
      help = 'DELTA is added to the calculated stack size' )

      
   parser.add_argument(
      '-verbose', 
      action='store_true',
      help = 'show the options used' )
      
   return parser

def analyse( 
   annotate, filt, 
   cpu, root_names, file_name, 
   stack_template, stack_output, 
   size, delta,
   verbose
):
   if verbose:
      print( "annotate       : %s" % annotate )
      print( "filt tool      : %s" % filt )
      print( "CPU            : %s" % cpu )
      print( "roots          : %s" % root_names )
      print( "lss file       : %s" % file_name )
      print( "stack template : %s" % stack_template )
      print( "stack file     : %s" % stack_output )
      print( "size override  : %s" % size )
      print( "size delta     : %s" % delta )   
       
   app = application( cpu, file_name, root_names, filt )
   if annotate:
      file_from_text( annotate, app.annotated_view() )
   app.handle_problems()
   if size != None:
      n = override
      print( "stack size override = %d" % override )
   else:
      n = delta + app.stack_use( root_names[ 0 ] )  
      print( "stack size is %d" % n )      
   file_from_text( 
      stack_output, 
      stripquotes( text_from_file( stack_template ) )
         .replace( 'STACK_SIZE', str( n )))

def run():
   parser = arguments_parser()
   results = parser.parse_args()
   analyse(
      results.annotate,
      results.filt,
      results.cpu,
      [ results.root ],
      results.input_file,
      results.template,
      results.output_file,
      results.size,
      results.delta,
      results.verbose
   )   

sys.setrecursionlimit( 100000 )      
threading.stack_size( 200000000 )
thread = threading.Thread( target = run )
thread.start()

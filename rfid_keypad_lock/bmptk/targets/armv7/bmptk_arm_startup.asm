                  #define mkt_fatal__unhandled_undefined_instruction  19
         #define mkt_fatal__unhandled_software_interrupt     20
         #define mkt_fatal__unhandled_prefetch_abort         21
         #define mkt_fatal__unhandled_data_abort             22
         #define mkt_fatal__unhandled_reserved               23
         #define mkt_fatal__unhandled_IRQ                    24
         #define mkt_fatal__unhandled_FIQ                    25
         #define mkt_fatal__memory_allocation_failure        26
         #define mkt_fatal__return_from_main                 27
         #define mkt_fatal__invalid_pin_number               28
         #define mkt_fatal__out_of_interrupt_slots           29

         //==================================================================
         //
         // default fatal error handler
         //
         // The default mkt_user_error_vector just jumps back to
         // mkt_fatal_error because there is nothing more usefull to do.
         // You can have the mkt_user_error_handler point to a routine
         // of your choice, but don't assume too much in that routine
         // (state of peripherals, processor state, availability of a
         // stack, etc.).
         //
         // TIP: Set a breakpoint on mkt_fatal_error to identify
         // fatal errors.
         // When it hits, examine R0 to find the cause.
         // The Insight debugger by default sets a breakpoint at 'exit',
         // hence that label is defined equal to mkt_fatal_error.
         //
         //==================================================================
               #if mkt_error_handling == 0
            // suppress all error handling code
            .macro mkt_fatal, NN
               // no error handling code
            .endm
         #endif
               #if mkt_error_handling != 0
            .macro mkt_fatal, NN
               mov     R0, #\NN
               bl      mkt_fatal_error
            .endm
         #endif
                  // start of a section of assembler code that can be placed in ROM
            .macro mkt_code
               .text
               .align
               .arm
            .endm
                     // start of a section for read/write,
            // explicitly initialised data
            .macro mkt_data
               .data
               .align
            .endm

            .macro mkt_uninitialized
               .section .uninitialized
               .align
            .endm
                     // start of a section for read/write,
            // 0-initialised data
            .macro mkt_bss
               .bss
               .align
            .endm
                     // start of a section for read-only, initialised data
            .macro mkt_rodata
               .text
               .align
            .endm
                     // this used to give problems with Insight, I have no idea why.
            // but with the new gcc and gdb these seem to be solved
            .macro mkt_code_separate_section, Name
                  .text
                  .section .text.\Name,"ax"
                  .arm
                  .align
            .endm


 
         
         //==================================================================
         //
         // ARM exception vectors
         //
         // The "vectors" section is mapped to a 64-byte memory area that is
         // handled in a special way: when running from ROM its content is
         // first copied to RAM and next that RAM area is remapped to
         // address 0.
         // This makes it possible for a running program to change the
         // exception (interrupt) vectors
         //
         //==================================================================

         // this section is handled in a special way by the link script
         .section vectors, "awx"
         .arm
         .align

         .global __startup
         __startup:

         // the ARM exception vectors, at physical address 0
         ldr     pc, mkt_reset_vector
         ldr     pc, mkt_undefined_instruction_vector
         ldr     pc, mkt_software_interrupt_vector
         ldr     pc, mkt_prefetch_abort_vector
         ldr     pc, mkt_data_abort_vector
         ldr     pc, mkt_reserved_vector
         .global irq_vec
         irq_vec:
         ldr     pc, mkt_reserved_vector // b _mkt_IRQ_interrupt_handler
         // ldr     pc, mkt_IRQ_vector
         ldr     pc, mkt_FIQ_vector

         .global mkt_reset_vector
         .global mkt_undefined_instruction_vector
         .global mkt_software_interrupt_vector
         .global mkt_prefetch_abort_vector
         .global mkt_data_abort_vector
         .global mkt_reserved_vector
         .global mkt_IRQ_vector
         .global mkt_FIQ_vector

         //==================================================================
         //
         // The following vectors are variables that contain the addresses
         // the various execptions will jump to. Change the content of a
         // vector to redirect the corresponding exception.
         //
         //==================================================================

         mkt_reset_vector:                    .word mkt_reset_handler
         mkt_undefined_instruction_vector:    .word 0
         mkt_software_interrupt_vector:       .word 0
         mkt_prefetch_abort_vector:           .word 0
         mkt_data_abort_vector:               .word 0
         mkt_reserved_vector:                 .word 0
         // mkt_IRQ_vector:                      .word unexp_exc_handler_6
         mkt_IRQ_vector:                      .word 0
         mkt_FIQ_vector:                      .word 0

         //==================================================================
         //
         // default unexpected exception handlers
         //
         // These handlers just load a unique value into R0 and jumps
         // to the mkt_fatal_error handler.
         //
         //==================================================================

         mkt_code



         mkt_reset_handler:
/*
               //==================================================================
         //
         // set stacks
         //
         // note: Early ARM ARM erroneously stated that msr cpsr_c, <imm>
         // is not an allowed form, and this error can still be found for
         // instance in Furber's book.
         // ref: http://www.arm.com/support/faqdev/1472.html
         //
         //==================================================================

         mkt_uninitialized
            .global _mkt_first_free
            _mkt_first_free:  .space 4

         mkt_code

         // get first free location
         ldr     r6, = _mkt_first_free
         ldr     r5, = mkt_ln_bss_beyond;
         // ldr     r5, [ r6 ]
         str     r5, [ r6 ]

         #define MKT_FIQ_STACK_SIZE 2048
         #define MKT_IRQ_STACK_SIZE 2048
         #define MKT_MAIN_STACK_SIZE 2048
         #define ROUND_UP( N, M ) ((N + (M-1)) & ~ (M-1))


         // select supervisor mode
         msr     cpsr_c, #0x13
         nop

         // select supervisor mode
         //msr     cpsr_c, #0xD3
         //nop

         // claim stack area
         add     r5, r5, #ROUND_UP(MKT_MAIN_STACK_SIZE, 4 )

         // set stack pointer
         mov     sp, r5

         // update the 'free ram' pointer
         str     r5, [ r6 ]
         
               //==================================================================
         // clear BSS (non-initialized variables)
         //==================================================================

         mov     r0, #0
         ldr     r1, = mkt_ln_bss_first
         ldr     r2, = mkt_ln_bss_beyond
      clear_bss_loop:
         cmp     r1, r2
         beq     clear_bss_beyond
         str     r0, [ r1 ], #4
         b       clear_bss_loop
      clear_bss_beyond:
      
                  //==================================================================
            // copy DATA segment (initialised data, RAM code) from ROM to RAM
            //==================================================================
               ldr     r1, = mkt_ln_data_rom_first
               ldr     r2, = mkt_ln_data_ram_first
               ldr     r3, = mkt_ln_data_ram_beyond
            copy_data_loop:
               cmp     r3, r2
               beq     copy_data_beyond
               ldr     r0, [ r1 ], #4
               str     r0, [ r2 ], #4
               b       copy_data_loop
            copy_data_beyond:
            

      

         




         //==================================================================
         //
         // select fast or slow GPIO access
         //
         //==================================================================

         #ifdef SCS
            #if mkt_fast_gpio == 1
               mov     r0, #3
            #else
               mov     r0, #0
            #endif
            ldr     r1, =SCS
            str     r0, [ r1 ]
         #endif


         //==================================================================
         // call main (= the application)
         //==================================================================
*/
         mov     r0, #0   // no arguments
         mov     r1, #0   // no argv either
         bl      main


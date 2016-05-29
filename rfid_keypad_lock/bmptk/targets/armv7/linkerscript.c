
/*
 * linkerscript
 */

OUTPUT_FORMAT(
   "elf32-littlearm",
   "elf32-bigarm",
   "elf32-littlearm"
)
OUTPUT_ARCH( arm )
ENTRY( __startup )

MEMORY {
      rom       : ORIGIN = 0x00000000, LENGTH = 0x00080000
      ram       : ORIGIN = 0x40000000, LENGTH = 0x00008000
}

mkt_ln_ram_beyond = 0x40008000;

SECTIONS {

   /* vectors: load into flash, but 'as if' at address 0 */
   .vectors : /* AT(0) */ {
      mkt_ln_vectors_rom_first = .;
      /* __vectors_start = . */
      *(vectors)
      . = ALIGN(64);
   } > rom
   /* mkt_vectors_load_addr = LOADADDR(.vectors); */
   mkt_ln_vectors_rom_beyond = .;

   /* instructions */
   .text :
   {
      mkt_ln_text_first = . ;

        . = ALIGN(0x4);
   /* PROVIDE(__ctors_start__ = .); */
   __ctors_start__ = .;
        KEEP (*_mkt__*.o(.ctors))
      KEEP (*(.ctors.crt_i_begin))
      KEEP (*(SORT(.ctors)))
      KEEP (*(.ctors.crt_end_n))

        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
   SORT(CONSTRUCTORS)
   /* PROVIDE(__ctors_end__ = .);   */
   __ctors_end__ = .;

        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        KEEP(*(.init))
        . = ALIGN(4);
__preinit_array_end = .;

        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        . = ALIGN(4);
        KEEP(*(.fini))
        __init_array_end = .;

       . = ALIGN(4);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;

       *(EXCLUDE_FILE (*text.iwram*) .text)
      *(.text)
      *(.text.*)
      *(.stub)
      *(.stub.*)
      /* .gnu.warning sections are          */
      /*    handled specially by elf32.em.  */
      *(.gnu.warning)
      *(.gnu.linkonce.t*)
      *(.glue_7 .glue_7t)

      KEEP (*(.init.crt_i_begin))
      KEEP (*(.init))
      KEEP (*(.init.crt_end_n))

      KEEP (*(.fini.crt_i_begin))
      KEEP (*(.fini))
      KEEP (*(.fini.crt_end_n))


      . = ALIGN(4);
   } > rom
   mkt_ln_text_beyond = . ;


   /* read-only data */
   .rodata :
   {
      mkt_ln_rodata_first = . ;

      *(.rodata)
      *all.rodata*(*)
      *(.roda)
      *(.rodata.*)
      *(.gnu.linkonce.r*)

      KEEP (*(.dtors.crt_i_begin))
      KEEP (*(SORT(.dtors)))
      KEEP (*(.dtors.crt_end_n))

      . = ALIGN(4);
   } > rom
   mkt_ln_rodata_beyond = . ;
   __ro_end = . ;

   .eh_frame :
   {
           KEEP (*(.eh_frame))

           . = ALIGN(4);
   } > ram

   .gcc_except_table :
   {
           *(.gcc_except_table)

           . = ALIGN(4);
   } > rom

   /* uninitialized data */
   .uninitialized : {
      . = ALIGN(4);
      *(.uninitialized)
      *(.uninitialized.*)
   } > ram

   /* read-write initialised data (DATA) */
   .data : {
      mkt_ln_data_ram_first = .;

      *(.data)
      *(.data.*)
      *(.gnu.linkonce.d*)
      CONSTRUCTORS

      . = ALIGN(4);
    } > ram AT > rom
    mkt_ln_data_ram_beyond = .;
    mkt_ln_data_rom_first  = LOADADDR( .data );

   /* read-write uninitialized reserved space (BSS) */
   .bss ( NOLOAD ): {
      . = ALIGN(4);
      mkt_ln_bss_first = .;
      *(.bss)
      *(.bss.*)
      *(.dynbss)
      *(.gnu.linkonce.b*)
      *(COMMON)
      . = ALIGN(4);
    } > ram
    mkt_ln_bss_beyond = . ;

    /* heap */
    _end = . ;
    __end__ = . ;
    PROVIDE (end = _end);

/*-----------------------------------*/

     /* STAB debugging sections */
     .stab                   0 : { *(.stab) }
     .stabstr                0 : { *(.stabstr) }
     .stab.excl              0 : { *(.stab.excl) }
     .stab.exclstr   0 : { *(.stab.exclstr) }
     .stab.index             0 : { *(.stab.index) }
     .stab.indexstr  0 : { *(.stab.indexstr) }
     .comment                0 : { *(.comment) }

     /* DWARF debug sections
        Symbols in the DWARF debugging sections are
        relative to the beginning
        of the section so we begin them at 0. */

     /* DWARF 1 */
     .debug                  0 : { *(.debug) }
     .line                   0 : { *(.line) }

     /* GNU DWARF 1 extensions */
     .debug_srcinfo  0 : { *(.debug_srcinfo) }
     .debug_sfnames  0 : { *(.debug_sfnames) }

     /* DWARF 1.1 and DWARF 2 */
     .debug_aranges  0 : { *(.debug_aranges) }
     .debug_pubnames 0 : { *(.debug_pubnames) }

     /* DWARF 2 */
     .debug_info             0 : { *(.debug_info) }
     .debug_abbrev   0 : { *(.debug_abbrev) }
     .debug_line             0 : { *(.debug_line) }
     .debug_frame    0 : { *(.debug_frame) }
     .debug_str              0 : { *(.debug_str) }
     .debug_loc              0 : { *(.debug_loc) }
     .debug_macinfo  0 : { *(.debug_macinfo) }

     /* SGI/MIPS DWARF 2 extensions */
     .debug_weaknames        0 : { *(.debug_weaknames) }
     .debug_funcnames        0 : { *(.debug_funcnames) }
     .debug_typenames        0 : { *(.debug_typenames) }
     .debug_varnames         0 : { *(.debug_varnames) }
}
      

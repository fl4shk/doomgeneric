
/* __flash =      0x00000000; */ /* 0x80000000; */
/* __flash_size = 0x00800000; */ /* 0x00018000; */
/* __ram =        0x00800000; */ /* 0x90000000; */
/* __ram_size   = 0x00800000; */ /* 0x01000000 */ /* 0x00010000; */
/* __heap_start = 0x01000000; */
/* __heap_size  = 0x01000000; */
/* __stack_size = 32k; */ /* 1k; */

/* INCLUDE picolibc.ld */


/* Default linker script, for normal executables */
/* Copyright (C) 2025 Free Software Foundation, Inc.
   Copying and distribution of this script, with or without modification,
   are permitted in any medium without royalty provided the copyright
   notice and this notice are preserved.  */
OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
OUTPUT_ARCH(riscv)
MEMORY
{
  /* rom (rx) : */
  /* ram (w!rx) : ORIGIN = 0x0000, LENGTH = 0x4000000 */
  /* ram (w!rx) : ORIGIN = 0x0000, LENGTH = 0x4000000 */ /* 64 MiB */
  ram (w!rx) : ORIGIN = 0x0000, LENGTH = 0x2000000 /* 32 MiB */
  /* framebuffer (w!rx) : ORIGIN = 0x2000000 */
  /* far_ram_0 (w!rx) : ORIGIN = 0x1000000, LENGTH = 0x1000 */
  /* far_ram_1 (w!rx) : ORIGIN = 0x2000000, LENGTH = 0x1000 */
  /* farthest_ram (w!rx) : ORIGIN = 0xfe000004, LENGTH = 0x8000 */
}
SECTIONS
{
  .text :
  {
    KEEP (*(SORT_NONE(.text.start)))
    KEEP (*(SORT_NONE(.init_array)))
    *(.text)
    KEEP (*(SORT_NONE(.fini_array)))
    *(.strings)
    etext = . ;
  }  > ram
  .tors :
  {
    __ctors = . ;
    *(.ctors)
    __ctors_end = . ;
    __dtors = . ;
    *(.dtors)
    __dtors_end = . ;
  } > ram
  .data :
  {
    __data_start = . ;
    __global_pointer$ = . ;
    /*  __data_source = LOADADDR(.) ;  */
    *(.data)
     __data_end = . ;
    /*  __data_size = __data_end - __data_start ;   */
  }  > ram
   PROVIDE(__data_source = LOADADDR(.data)) ;
   PROVIDE(__data_size = __data_end - __data_start) ;
  .rodata :
  {
     __rodata_start = . ;
    /*  __rodata_source = LOADADDR(.) ;  */
    *(.rodata)
     __rodata_end = . ;
    /*  __rodata_size = __rodata_end - __rodata_start ;   */
  }  > ram
   PROVIDE(__rodata_source = LOADADDR(.rodata)) ;
   PROVIDE(__rodata_size = __rodata_end - __rodata_start) ;
  .bss :
  {
     __bss_start = . ;
    *(.bss)
    *(.sbss)
    *(COMMON)
     __bss_end = . ;
    /*  __bss_size = __bss_end - __bss_start ;   */
  }  > ram
   PROVIDE(__bss_size = __bss_end - __bss_start) ;
  /*  PROVIDE (stack = 0x03fffffc) */
  .stack  0x0fffffc /*0x30000*/   :
  {
     __stack = . ;
    *(.stack)
  }  > ram
  .heap  0x1000000   :
  {
    /* 16 MiB of heap */
     __heap_start = . ;
     __heap_end = 0x0fffffc ;
  }  > ram
  .stab 0 (NOLOAD) :
  {
    *(.stab)
  }
  .stabstr 0 (NOLOAD) :
  {
    *(.stabstr)
  }
  /*
  .far_0 :
  {
     __far_0_start = . ;
    *(.far_0)
     __far_0_end = . ;
     __far_0_size = . - __far_0_start ;
  }  > far_ram_0
  .far_1 :
  {
     __far_1_start = . ;
    *(.far_1)
     __far_1_end = . ;
     __far_1_size = . - __far_1_start ;
  }  > far_ram_1
  .farthest :
  {
     __farthest_start = . ;
    *(.farthest)
     __farthest_end = . ;
     __farthest_size = . - __farthest_start ;
  }  > farthest_ram
  */
}

/* Reworking to run on GR-LYCHEE in boot mode 1 */

MEMORY {
    ROM          (rx)  : ORIGIN = 0x00000000, LENGTH = 32M
    SFLASH       (rx)  : ORIGIN = 0x18000000, LENGTH = 0x800000
    SYSTEM_RAM   (rwx) : ORIGIN = 0x20020000, LENGTH = 0x04000
    STACK        (rw)  : ORIGIN = 0x20024000, LENGTH = 0x14000
    CACHED_RAM   (rw)  : ORIGIN = 0x20039000, LENGTH = 0x001C7000
    UNCACHED_RAM (rw)  : ORIGIN = 0x60200000, LENGTH = 0x00100000
}

SECTIONS
{
    .text : {

        . = 0x00000000;
        * (VECTOR_TABLE)            /*asm*/

        . = 0x00000200;
        * (RESET_HANDLER)           /*asm*/
        * (INIT_TTB)                /*asm*/
        * (INITCA9CACHE)            /*asm*/

        * (CODE_BASIC_SETUP)
        */peripheral_init_basic.o (.text .text.*)
        */bsc.o (.text .text.*)
        */bsc_userdef.o (.text .text.*)

        * (CONST_BASIC_SETUP)
        */peripheral_init_basic.o (.rodata .rodata.*)
        */bsc.o (.rodata .rodata.*)
        */bsc_userdef.o (.rodata .rodata.*)

        * (CODE_IRQ_HANDLER)        /*asm*/

        * (InRoot$$Sections)

        * (CODE_FPU_INIT)           /*nothing*/
        * (FPU_INIT)                /*asm*/

        * (CONST_FPU_INIT)          /*nothing*/

        * (CODE_RESET)              /*asm*/
        */initsect.o (.rodata .rodata.*)
        */port_init.o (.text .text.*)
        */stb_init.o (.text .text.*)
        */resetprg.o (.text .text.*)
        */l2_cache_init.o (.text .text.*)
        * (INIT_VBAR)               /*asm*/

        * (CONST_RESET)             /*asm*/
        */sections.o (.rodata .rodata.*)
        */port_init.o (.rodata .rodata.*)
        */stb_init.o (.rodata .rodata.*)
        */resetprg.o (.rodata .rodata.*)
        */l2_cache_init.o (.rodata .rodata.*)
    } > SFLASH

    /*
        The vector mirror table and the irq handler must include in the .data section.
        Because the addresses on ROM are not calculated if the two put in each sections.
    */
    .data : {
        . = 0x00000000;
        __vect_load  = .;
        __vect_start = LOADADDR(.data) + ( __vect_load - ADDR(.data) );

        Image$$VECTOR_MIRROR_TABLE$$Base = .;
        * (VECTOR_MIRROR_TABLE)         /*asm*/

        __vect_end =  LOADADDR(.data) + ( . - ADDR(.data) );

        /********************************************************************/

        . = 0x00000100;
        __fastcode_load  = .;
        __fastcode_start = LOADADDR(.data) + ( __fastcode_load - ADDR(.data) );

        * (CODE_HANDLER_JMPTBL)
        */intc_userdef.o (.text .text.*)

        * (CONST_HANDLER_JMPTBL)
        */intc_userdef.o (.rodata .rodata.*)

        * (IRQ_FIQ_HANDLER)         /*asm*/
        * (CODE_HANDLER)
        */intc_handler.o (.text .text.*)

        * (CONST_HANDLER)
        */intc_handler.o (.rodata .rodata.*)

        * (CODE_IO_REGRW)
        */rza_io_regrw.o (.text .text.*)

        * (CONST_IO_REGRW)
        */rza_io_regrw.o (.rodata .rodata.*)

        __fastcode_end = LOADADDR(.data) + ( . - ADDR(.data) );

        /********************************************************************/

        __data_load  = .;
        __data_start = LOADADDR(.data) + ( __data_load - ADDR(.data) );

        * (DATA_HANDLER_JMPTBL)
        */intc_userdef.o (.data)

        * (DATA_BASIC_SETUP)
        */peripheral_init_basic.o (.data)
        */bsc.o (.data)
        */bsc_userdef.o (.data)

        * (DATA_HANDLER)
        */intc_handler.o (.data)

        * (DATA_FPU_INIT)           /*nothing*/

        * (DATA_RESET)
        */port_init.o (.data)
        */stb_init.o (.data)
        */resetprg.o (.data)
        */l2_cache_init.o (.data)

        * (DATA_IO_REGRW)
        */rza_io_regrw.o (.data)

        * (.data .data.*)

        __data_end = LOADADDR(.data) + ( . - ADDR(.data) );

    } > CACHED_RAM AT > SFLASH
    
    .bss : {
        __bss_start = .;

        * (BSS_HANDLER_JMPTBL)
        */intc_userdef.o (.bss)

        * (BSS_BASIC_SETUP)
        */peripheral_init_basic.o (.bss)
        */bsc.o (.bss)
        */bsc_userdef.o (.bss)

        * (BSS_HANDLER)
        */intc_handler.o (.bss)

        * (BSS_FPU_INIT)            /*nothing*/

        * (BSS_RESET)
        */port_init.o (.bss)
        */stb_init.o (.bss)
        */resetprg.o (.bss)
        */l2_cache_init.o (.bss)

        * (BSS_IO_REGRW)
        */rza_io_regrw.o (.bss)

        __bss_end = .;
    } > CACHED_RAM


    .stack : {
        . = ALIGN( 0x10 );
        Image$$ARM_LIB_STACK$$ZI$$Base = .;
        . += 0x00008000;
        Image$$ARM_LIB_STACK$$ZI$$Limit = .;

        . = ALIGN( 0x10 );
        Image$$IRQ_STACK$$ZI$$Base = .;
        . += 0x00002000;
        Image$$IRQ_STACK$$ZI$$Limit = .;

        . = ALIGN( 0x10 );
        Image$$FIQ_STACK$$ZI$$Base = .;
        . += 0x00002000;
        Image$$FIQ_STACK$$ZI$$Limit = .;

        . = ALIGN( 0x10 );
        Image$$SVC_STACK$$ZI$$Base = .;
        . += 0x00002000;
        Image$$SVC_STACK$$ZI$$Limit = .;

        . = ALIGN( 0x10 );
        Image$$ABT_STACK$$ZI$$Base = .;
        . += 0x00002000;
        Image$$ABT_STACK$$ZI$$Limit = .;

        . = ALIGN( 0x4000 );
        Image$$TTB$$ZI$$Base = .;
        . += 0x00004000;
        Image$$TTB$$ZI$$Limit = .;
    } > STACK

    .uncached_RAM (NOLOAD) : {
        * (BSS_RIIC_SAMPLE)                 /* RIIC sample transfer work area */
        * (BSS_RSPI_SAMPLE)                 /* RSPI sample transfer work area */
        * (BSS_SCIF_SYNC_SAMPLE)            /* SCIF_SYNC sample work area */
        * (BSS_DMAC_SAMPLE_SDRAM)           /* DMAC sample transfer destination work area */
        * (BSS_NANDNC)                      /* FLCTL work */
    } > UNCACHED_RAM

    .uncached_RAM2 : {
        __cache_operation_load  = .;
        __cache_operation_start = LOADADDR(.uncached_RAM2) + ( __cache_operation_load - ADDR(.uncached_RAM2) );

        * (L1_CACHE_OPERATION)
        * (CODE_CACHE_OPERATION)
        */cache.o (.text .text.*)

        * (CONST_CACHE_OPERATION)
        */cache.o (.rodata .rodata.*)

        * (DATA_CACHE_OPERATION)
        */cache.o (.data)

        * (BSS_CACHE_OPERATION)
        */cache.o (.bss)

        __cache_operation_end = LOADADDR(.uncached_RAM2) + ( . - ADDR(.uncached_RAM2) );
    } > UNCACHED_RAM AT > SFLASH

    .text2 : {
        * (.text .text.*)
        * (.rodata .rodata.*)
    } > SFLASH


    .data2 : {
        __data2_load  = .;
        __data2_start = LOADADDR(.data2) + ( __data2_load - ADDR(.data2) );

        * (.data .data.*)

        __data2_end = LOADADDR(.data2) + ( . - ADDR(.data2) );
    } > CACHED_RAM AT > SFLASH


    .bss2 : {
        __bss2_start = .;

        * (BSS_DMAC_SAMPLE_INTERNAL_RAM)    /* DMAC sample transfer source work area */

        * (.bss .bss.*)
        * (COMMON)

        __bss2_end = .;

        . = ALIGN( 0x4 );

        end = .;
        . = . + 0x00080000;
        stack_ptr = .;

    } > CACHED_RAM


    .dummy : {
        . = 0x0100;
    } > SFLASH = 0x00
}

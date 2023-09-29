/* Entry Point */
ENTRY(Reset_Handler)


/*
 * The '__stack' definition is required by crt0, do not remove it.
 */
__stack = ORIGIN(RAM) + LENGTH(RAM);

/* Highest address of the user mode stack */
_estack = __stack; 	/* STM specific definition */


_Min_Heap_Size = 0x200 ;	/* required amount of heap  */
_Min_Stack_Size = 0x1000; /* 0x400 */ ;	/* required amount of stack */

__Main_Stack_Size = _Min_Stack_Size;

/* app zone size - 256K*/
/* _app_zone_size = 208K ; /*

/* app offset */
_app_offset = 0x0 ;
/* _app_offset = 0xC000 ; */

/* isr size */
/*_isr_vector_size = 0x190 ;*/
_isr_vector_size = 0 ; /* 84*4 = 336 */

/* fw info size */
_fw_info_size = 0 ;


/*
 * Default heap definitions.
 * The heap start immediately after the last statically allocated 
 * .sbss/.noinit section, and extends up to the main stack limit.
 */
PROVIDE ( _Heap_Begin = _end_noinit ) ;
PROVIDE ( _Heap_Limit = __stack - __Main_Stack_Size ) ;



/* Specify the memory areas */
MEMORY
{
    CCMRAM              (xrw)    : ORIGIN = 0x10000000,                                                   LENGTH = 0K
    RAM                 (xrw)    : ORIGIN = 0x20000000,                                                   LENGTH = 96K
    FLASH               (rx)    : ORIGIN = (0x8000000 + _app_offset + _isr_vector_size + _fw_info_size),  LENGTH = (_app_zone_size - _isr_vector_size - _fw_info_size)
}


/* Define output sections */

SECTIONS
{
    /* The startup code goes first into FLASH */
	.isr_vector :
	{
		. = ALIGN(4);
		KEEP(*(.isr_vector)) /* Startup code */
		. = ALIGN(4);
	} >FLASH

    .data_keep :
    {
        . = ALIGN(4);
        KEEP(*(.data_keep))
        . = ALIGN(4);
    } >FLASH

    /* The program code and other data goes into FLASH */
    .text :
    {
        . = ALIGN(4);
        *(.text)           /* .text sections (code) */
        *(.text*)          /* .text* sections (code) */
        *(.glue_7)         /* glue arm to thumb code */
        *(.glue_7t)        /* glue thumb to arm code */
        *(.eh_frame)

        KEEP (*(.init))
        KEEP (*(.fini))

        . = ALIGN(4);
        _etext = .;        /* define a global symbols at end of code */
    } >FLASH

    /* Constant data goes into FLASH */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)         /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
        . = ALIGN(4);
    } >FLASH

    .ARM.extab   : { 
        . = ALIGN(4);
        *(.ARM.extab* .gnu.linkonce.armextab.*) 
        . = ALIGN(4);
        } >FLASH

    .ARM : 
    {
        . = ALIGN(4);
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
        . = ALIGN(4);
    } >FLASH

    .preinit_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(4);
    } >FLASH
    
    .init_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(4);
    } >FLASH
    
    .fini_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
        . = ALIGN(4);
    } >FLASH

    /* used by the startup to initialize data */
    _sidata = LOADADDR(.data);

    /* Initialized data sections goes into RAM, load LMA copy after code */
    .data : 
    {
        . = ALIGN(4);
        _sdata = .;        /* create a global symbol at data start */
        *(.data)           /* .data sections */
        *(.data*)          /* .data* sections */
        *(.RamFunc)        /* .RamFunc sections */
        *(.RamFunc*)       /* .RamFunc* sections */

        . = ALIGN(4);
        _edata = .;        /* define a global symbol at data end */
    } >RAM AT> FLASH 

    /* Uninitialized data section */
    . = ALIGN(4);
    .bss :
    {
        /* This is used by the startup in order to initialize the .bss secion */
        _sbss = .;         /* define a global symbol at bss start */
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);
        _ebss = .;         /* define a global symbol at bss end */
        __bss_end__ = _ebss;
    } >RAM

    .noinit (NOLOAD) : ALIGN(4)
    {
        _noinit = .;
        
        *(.noinit .noinit.*) 
        
         . = ALIGN(4) ;
        _end_noinit = .;   
    } > RAM
    
    /* Mandatory to be word aligned, _sbrk assumes this */
    PROVIDE ( end = _end_noinit ); /* was _ebss */
    PROVIDE ( _end = _end_noinit );
    PROVIDE ( __end = _end_noinit );
    PROVIDE ( __end__ = _end_noinit );
    
    /* User_heap_stack section, used to check that there is enough RAM left */
    ._user_heap_stack :
    {
        . = ALIGN(4);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;
        . = ALIGN(4);
    } >RAM


    /* CCM-RAM section
    *
    * IMPORTANT NOTE!
    * If initialized variables will be placed in this section,
    * the startup code needs to be modified to copy the init-values.
    */

    /* uninitialized CCRAM objects (like, buffers) */
    .ccmram_bss :
    {
        __ccram_start_bss__ = .; /* define a global symbol at ccram start */
        _sccmram_bss = __ccram_start_bss__; 
        KEEP(*(.ccmram_bss))
        KEEP(*(.ccmram_bss*))
        . = ALIGN(4);
        __ccram_end_bss__ = .; /* define a global symbol at end of *used* CCRAM (BSS) */
        _eccmram_bss = __ccram_end_bss__; 
    } >CCMRAM

    /* initialized CCRAM objects (like, initialized variables) */
        _siccmram_data = LOADADDR(.ccmram_data);
    .ccmram_data :
    {
        . = ALIGN(4);
        _sccmram_data = .;       /* create a global symbol at ccmram start */
        *(.ccmram_data)
        *(.ccmram_data*)

        . = ALIGN(4);
        _eccmram_data = .;       /* create a global symbol at ccmram end */
    } >CCMRAM AT> FLASH


    /* Remove information from the standard libraries */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )

    }

    .ARM.attributes 0 : { *(.ARM.attributes) 
    }
}

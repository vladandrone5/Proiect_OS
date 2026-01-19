ENTRY(_start)

SECTIONS {
    . = 0x80200000; /* Adresa de start în RAM (QEMU load addr) */
    .text : { /*copies from .text.entry in input object files to .text*/
        *(.text.entry)
        *(.text*)
    }
    .rodata : ALIGN(4){ *(.rodata*) }
    .data : ALIGN(4){ *(.data*) } /*collects initialized data*/

    . = 0x80300000;
    .bss : ALIGN(4) { 
        _bss_start = .;
        *(.bss*)
        *(COMMON)
        _bss_end = .;
    } /*for unitialized*/

    /* ---------------------------
     * Stack — 1 MB
     * --------------------------- */
    . = 0x80400000;
    stack_bottom = .;
    . = . + (1 * 1024 * 1024);  /* 1MB stack */
    stack_top = .;    

    . = 0x80500000;
    .proc_ctxs (NOLOAD) : {
        __proc_ctxs_start = .;
        *(.proc_ctxs)
        . = ALIGN(16);
        __proc_ctxs_end = .;
    }

    . = 0x80D00000;
    . = ALIGN(16);
    HEAP_START = .;
    . = . + (1024 * 1024 * 1024);
    HEAP_END = .;
}
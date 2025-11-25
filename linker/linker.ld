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
        . = . + (1024 * 1024);
        _bss_end = .;
    } /*for unitialized*/

     . = ALIGN(16);
    heap_start = .;
    . = . + (1 * 1024 * 1024);  /* 1MB heap */
    heap_end = .;

    /* ---------------------------
     * Stack — 1 MB
     * We'll place stack ABOVE heap.
     * --------------------------- */
    . = ALIGN(16);
    stack_bottom = .;
    . = . + (1 * 1024 * 1024);  /* 1MB stack */
    stack_top = .;    
}
.option norvc # no risc-v compression, so that all instructions remain 32-bit throught compilation

# .equ BOOTLOADER_ADDR, 0x80200000
.equ UART_ADDR, 0x10000000

.section .rodata
boot_msg: .string "\n\r###################################\n\r#\t\t\t\t  #\n\r#\tBooting EdgarieOS...\t  #\n\r#\t\t\t\t  #\n\r###################################\n\r"

.section .text # .text - execute block
.globl _start
.globl kmain
.globl stack_top
.globl _bss_start
.globl _bss_start


.p2align 2 #allign all instructions at offsets of 4 bytes

_start:

    # should also make sure we use a id#0 set hart when working with the os

    # make sure global pointer is set so that all immediate calls work fine
    # norelax removes relative addresive to gb, so that we can set gb normally
    # .option push
    # .option norelax
	#     la		gp, _global_pointer$
    # .option pop
    # not needed ?

    la sp, stack_top # Set stack pointer to the top of the 1MB stack

    jal _clear_bss # set bss area to 0

    # jal set_init_direct_ihs # set mtvec at the start before enabling vectored interrupts

    la a0, boot_msg
    jal puts # write booting message

    jal kmain
    
1:  j 1b # loop in case kmain somehow exits loop

_clear_bss:
    la a0, _bss_start
    la a1, _bss_end
2:   
    beq a0,a1,3f
    sw zero,0(a0)
    addi a0,a0,4
3:
    ret

puts:
    mv t0, a0
    addi sp,sp,-4
    sw ra,0(sp)
loop_puts:
    lbu t1,0(t0)     
    beqz t1, end_puts 
    jal putc  # jump to _putc and save position to ra
    addi t0, t0, 1
    j loop_puts
end_puts:
    lw ra,0(sp)
    addi sp,sp,4
    ret
    
putc:
    li t2, UART_ADDR
    sb t1, 0(t2)
    ret

_fill_space: .space 0x1FE - (. - _start) , 0 #gnu directives fill space till 508
_signature: .word 0x0000AA55
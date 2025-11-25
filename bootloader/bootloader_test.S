.option norvc # no risc-v compression, so that all instructions remain 32-bit throught compilation

# .equ BOOTLOADER_ADDR, 0x80200000
.equ UART_ADDR, 0x10000000
.equ CLINT_MSIP_ADDR, 0x2000000  # Standard address for MSIP register

.section .rodata
boot_msg: .string "\n\r###################################\n\r#\t\t\t\t  #\n\r#\tBooting EdgarieOS...\t  #\n\r#\t\t\t\t  #\n\r###################################\n\r"
dirs_msg: .string "\n\rInterrupt subroutine called...\n\r"
except_msg: .string "\n\r*** Exception/Fault Trap Taken! ***\n\r"

.section .text # .text - execute block
.globl _start
.globl kmain
.globl stack_top
.globl _bss_start
.globl _bss_start


.p2align 2 #allign all instructions at offsets of 4 bytes

_start:

    la sp, stack_top # Set stack pointer to the top of the 1MB stack

    jal _clear_bss # set bss area to 0

    jal clear_msip_flag # Clear the Software Interrupt Pending flag (MSIP) in CLINT

    jal set_init_direct_ihs # set mtvec at the start before enabling vectored interrupts

    la a0, boot_msg
    jal puts # write booting message

    jal kmain
    
1:  j 1b # loop in case kmain somehow exits loop

clear_msip_flag:
    # Clear the Machine Software Interrupt Pending (MSIP) flag in the CLINT.
    # The MSIP register is typically a 4-byte word at CLINT_MSIP_ADDR.
    li t0, 0 # value to write (0 to clear)
    li t1, CLINT_MSIP_ADDR # address of MSIP register for HART 0
    sw t0, 0(t1) # write 0 to the MSIP register
    ret

_clear_bss:
    la a0, _bss_start
    la a1, _bss_end
2:   
    beq a0,a1,3f
    sw zero,0(a0)
    addi a0,a0,4
3:
    ret

set_init_direct_ihs: # interrupt handler subroutine
    # csrc mtvec, 1 is used to clear the MODE bit (to ensure Direct Mode 00).
    csrc mtvec, 1 
    la t0,init_dirs
    csrw mtvec, t0
    ret


# ------------------------------------------------------------------------------
# init_dirs: The Robust Machine Trap Handler
# ------------------------------------------------------------------------------
init_dirs: 
    # The minimum required stack frame for M-mode traps on RV32
    # We save a handful of temp registers and the return address, RA (x1).
    # Since we call 'puts', we need to save all registers that 'puts' or its callers might clobber.
    addi sp, sp, -32
    sw ra, 0(sp)   # x1 (ra)
    sw t0, 4(sp)   # x5 (t0)
    sw t1, 8(sp)   # x6 (t1)
    sw t2, 12(sp)  # x7 (t2)
    sw a0, 16(sp)  # x10 (a0)
    sw a1, 20(sp)  # x11 (a1)
    sw mcause, 24(sp) # Save mcause register for inspection
    sw mepc, 28(sp)   # Save mepc register for inspection/update

    # 1. Check mcause: Bit 31 (Interrupt flag)
    # Read mcause into t0
    csrr t0, mcause 
    
    # Check if MSB (Interrupt bit) is set (t0 < 0 is equivalent to MSB=1)
    bltz t0, handle_interrupt
    
    # ------------------------------------
    # Handle EXCEPTION (Not an Interrupt)
    # ------------------------------------
handle_exception:
    # Print exception message
    la a0, except_msg
    jal puts

    # For synchronous exceptions, we MUST advance mepc by the instruction length (4 bytes).
    csrr t0, mepc
    addi t0, t0, 4
    csrw mepc, t0
    
    j restore_context

    # ------------------------------------
    # Handle INTERRUPT
    # ------------------------------------
handle_interrupt:
    # Print interrupt message
    la a0, dirs_msg
    jal puts

    # Read mcause again (t0 already holds it)
    csrr t0, mcause

    # Check for Machine Software Interrupt (MCause value is 0x80000003)
    li t1, 3 # Exception code for Machine Software Interrupt (3)
    and t0, t0, 0x7FFFFFFF # Clear MSB (Interrupt bit)
    bne t0, t1, unknown_interrupt # If not a Software Interrupt, skip MSIP clear

    # Clear MSIP flag in CLINT (prevents the loop)
    li t0, CLINT_MSIP_ADDR
    li t1, 0
    sw t1, 0(t0) 

unknown_interrupt:
    # No need to advance mepc for interrupts, as the hardware handles the return address.
    
    # Fall through to restore_context
    
    # ------------------------------------
    # Restore Context and Return
    # ------------------------------------
restore_context:
    lw mepc, 28(sp) # Restore mepc 
    lw mcause, 24(sp) # Restore mcause 
    lw a1, 20(sp)  # Restore x11 (a1)
    lw a0, 16(sp)  # Restore x10 (a0)
    lw t2, 12(sp)  # Restore x7 (t2)
    lw t1, 8(sp)   # Restore x6 (t1)
    lw t0, 4(sp)   # Restore x5 (t0)
    lw ra, 0(sp)   # Restore x1 (ra)
    addi sp, sp, 32
    
    mret

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
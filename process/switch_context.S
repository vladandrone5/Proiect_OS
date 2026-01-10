.globl switch_context

switch_context:

    # csrrw  zero, sscratch , a0 # saving current/old process sp for later data saving implementations

    addi sp,sp,-64

    sw ra,   0(sp)
    sw s0,   4(sp)
    sw s1,   8(sp)
    sw s2,  12(sp)
    sw s3,  16(sp)
    sw s4,  20(sp)
    sw s5,  24(sp)
    sw s6,  28(sp)
    sw s7,  32(sp)
    sw s8,  36(sp)
    sw s9,  40(sp)
    sw s10, 44(sp)
    sw s11, 48(sp)

    sw sp, 8(a0)
    mv a0, a1

    j load_context
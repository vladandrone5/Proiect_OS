#include "interrupts.h"
#include "../timer/timer.h"
#include "../misc/csr.h"
#include "../uart/uart.h"
#include "../plic/plic.h"
#include "../keyboard/keyboard.h"
#include "../process/process.h"

void stvec_idt(void)
{
    __asm__ volatile (
        ".org  stvec_idt + 5*4;"
        "jal   zero,sti_handler;"
        ".org  stvec_idt + 9*4;"
        "jal   zero,seie_handler;" 
        :
        :
        : 
        );
}

void setup_timer_int_csrs(u32 os_freq)
{
    get_mtime();
    set_mtimecmp(os_freq);
}

void reset_system_interrupts(void)
{
    clear_bit_csr_sstatus(SSTATUS_SIE_MASK); /* disable all interrupts*/
    write_csr_sie(SIE_DISABLE_MASK); /*clear all int enable bits*/
    write_csr_satp(SATP_DISABLE_MASK); /* clear virtual paging reg(required for supervisor mode)?*/
}

void reset_plic_interrupts(void)
{
    reset_all_plic_source_pry();
    reset_all_plic_int_en(CTX_1_IEB_ADDR);
    reset_plic_pry_trsh(CTX_1_PRY_TRSH_ADDR);
}

void reset_interrupts(void)
{
    reset_system_interrupts(); /*refers to m/s csr's*/
    reset_plic_interrupts(); /*refers to plic interrupts data*/
}

void system_interrupts_init(u32 sie_mask)
{
    write_csr_stvec((u32)stvec_idt | STVEC_MODE_MASK); /*it is recommended to have interrupts diabled before changing mtvec*/
    write_csr_sie(sie_mask);
    write_csr_sstatus(read_csr_sstatus() | SSTATUS_SIE_MASK);
}

void plic_interrupts_init(void)
{
    set_plic_pry_trsh(CTX_1_PRY_TRSH_ADDR, 1);

    setup_keyboard_interrupt(KEYBOARD_SRC_ID,KEYBOARD_INT_PRY,CTX_1_IEB_ADDR);
    enable_uart_rx_interrupt();
}

void interrupts_init(void)
{
    setup_timer_int_csrs(FREAKUENCY);

    system_interrupts_init(SIE_SEIE_MASK | SIE_STIE_MASK);
    plic_interrupts_init();
}

#pragma GCC push_options
#pragma GCC optimize ("align-functions=4")

void sti_handler(void) 
{
    if(kernel_rpc == 0)
    {
        kernel_rpc = read_csr_sepc();
    }

    schedule(); 

    ++ticks;
    setup_timer_int_csrs(FREAKUENCY);
}

#pragma GCC pop_options

#pragma GCC push_options
#pragma GCC optimize ("align-functions=4")

void seie_handler(void) 
{
    u32 interrupts_id = read_claim_plic_int_msg(CTX_1_CLCO_ADDR);
    

    switch (interrupts_id)
    {
    case 10: 
        {
            u8 character = uart_readchar();
            process_keys(character);
            break;
        }   
    default: {uart_printf((const u8 *)"Unknown interrupt handled:ID{%u}:\n",interrupts_id); break;}
    }
    
    send_complete_plic_int_ID(CTX_1_CLCO_ADDR,interrupts_id);
}

#pragma GCC pop_options





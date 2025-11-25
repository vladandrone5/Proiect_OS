#include "kmain.h"
#include "../misc/types.h"
#include "../timer/timer.h"
#include "../uart/uart.h"
#include "../interrupts/interrupts.h"
#include "../sbi/sbi.h"

void kmain(void) {
    //clear_screen();


	// interrupts_init();
	uart_prints((const u8 *)"Start of kmain...\r\n");
	uart_printf((const u8 *)"IDT Addr(dec):%u\n\r",(u32)stvec_idt);

	reset_interrupts();
    get_mtime();
	sbi_set_timer(FREAKUENCY);
	// set_mtimecmp(FREAKUENCY);

	interrupts_init();

	uart_prints((const u8 *)"ceva ceva\r\n");
	uart_printf((const u8 *)"data:%u\n\r",(u32)11);
	while(1) {
		// Read input from the UART
		//uart_printf((const u8 *)"Ticks:%d\n\r",(u32)ticks);
		__asm__ volatile ("wfi");
		//uart_putchar(*UART_ADDR);
	}
	return;
}
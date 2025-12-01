#include "kmain.h"
#include "../misc/types.h"
#include "../uart/uart.h"
#include "../interrupts/interrupts.h"

void kmain(void) {
    //clear_screen();

	u8 msg[] = "Start of kmain...";
	// interrupts_init();
	uart_printf((const u8 *)"%s\n",msg);
	uart_printf((const u8 *)"IDT Addr(dec):%u\n\r",(u32)stvec_idt);

	reset_interrupts();
	interrupts_init();

	uart_prints((const u8 *)"ceva ceva\r\n");
	uart_printf((const u8 *)"data:%d\n\r",(i32)-11);
	while(1) {
		// Read input from the UART
		// uart_printf((const u8 *)"Time:%u\n",(u32)timer);
		// uart_prints((const u8 *)"test...\n\r");
		//uart_putchar(*UART_ADDR);
	}
	return;
}
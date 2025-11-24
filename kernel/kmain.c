#include "kmain.h"
#include "../misc/types.h"
#include "../timer/timer.h"
#include "../uart/uart.h"
#include "../interrupts/interrupts.h"

void kmain(void) {
    clear_screen();

	// get_mtime();
	// set_mtimecmp(FREAKUENCY);
	// interrupts_init();

	uart_prints((const u8 *)"ceva ceva\r\n");
	while(1) {
		// Read input from the UART
		//uart_printf((const u8 *)"Ticks:%d\n",ticks);
		//uart_putchar(*UART_ADDR);
	}
	return;
}
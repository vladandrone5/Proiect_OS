#include "kmain.h"

void kmain(void) {
    clear_screen();

	interrupts_init();

	uart_prints("Pisi caca\r\n");
	while(1) {
		// Read input from the UART
		uart_printf("Ticks:%d\n",ticks);
		//uart_putchar(*UART_ADDR);
	}
	return;
}
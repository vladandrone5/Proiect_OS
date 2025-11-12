#include "kmain.h"

void kmain(void) {
    clear_screen();
	uart_print("Hello world!\r\n");
	while(1) {
		// Read input from the UART
		uart_putchar(*UART_ADDR);
	}
	return;
}
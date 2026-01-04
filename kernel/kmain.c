#include "kmain.h"
#include "../misc/types.h"
#include "../uart/uart.h"
#include "../interrupts/interrupts.h"
#include "../process/process.h"
#include "../misc/csr.h"

void kmain(void) {
    //clear_screen();

	u8 msg[] = "Start of kmain...";
	// interrupts_init();
	initialize_processes();
	uart_printf((const u8 *)"%s\n",msg);

	reset_interrupts();
	uart_prints((const u8 *)"Reseted ints...\n");
	interrupts_init();

	while(1) {
	}
	return;
}
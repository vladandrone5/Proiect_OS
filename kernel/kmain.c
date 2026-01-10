#include "kmain.h"
#include "../misc/types.h"
#include "../uart/uart.h"
#include "../interrupts/interrupts.h"
#include "../process/process.h"
#include "../misc/csr.h"
#include "../terminal/sys_cmd.h"

void kmain(void) {

	initialize_processes();
	reset_interrupts();
	interrupts_init();

	_shell();
	while(1) {}
	return;
}
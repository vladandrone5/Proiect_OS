#include "kmain.h"
#include "../misc/types.h"
#include "../uart/uart.h"
#include "../interrupts/interrupts.h"
#include "../process/process.h"
#include "../misc/csr.h"
#include "../terminal/sys_cmd.h"
#include "../memory/memory.h"

void kmain(void) {

	reset_interrupts();
	initialize_processes();
	init_heap();
	interrupts_init();

	_shell();
	while(1) {}
	return;
}
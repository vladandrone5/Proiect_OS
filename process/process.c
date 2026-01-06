#include "process.h"
#include "../string/string.h"
#include "../misc/csr.h"
#include "../uart/uart.h"

u8 process_finished_running = 0;
u8 last_used_id = 0;
u8 active_processes = 0;
u8 current_process = 0;
u8 process_runtime = 0;
process process_context[8];
u32 kernel_rpc = 0;

u8 save_context(process *active_process);
u8 load_context(process *active_process);

u8 get_new_id(u8 id) /* gets new id if the curretn one is used */
{
	u8 new_id_used = 0;
	for (u8 new_id = 1; new_id < 256; new_id++)
	{
		new_id_used = 0;
		for (u8 pr = 0; pr < 8; pr++)
		{
			if (process_context[pr].state == PROCESS_ACTIVE && process_context[pr].process_id == id)
			{
				new_id_used = 1;
				break;
			}
		}

		if (!new_id_used)
		{
			return new_id;
		}
	}
}

void initialize_processes(void)
{
	for (u8 pr = 0; pr < 8; pr++)
	{
		process_context[pr].process_id = 0;
		process_context[pr].time_slice = 0;
		process_context[pr].state = PROCESS_INACTIVE;
		strncpy(process_context[pr].process_name, (const u8 *)"", 1);

		for (u8 reg_i = 0; reg_i < 32; reg_i++)
		{
			process_context[pr].env.x[reg_i] = 0;
		}
	}
}

void process_done(void)
{
	// TO DO: call remove process here
	process_finished_running = 1;

	__asm__ volatile("wfi");
}

u8 add_process(u8 id, u8 priority, const u8 *process_name)
{
	// check if id used(shouldnt be), if there is enough space for a new process
	if (active_processes == 255)
	{
		return ERR_PBF;
	}

	u8 return_value = 0;
	for (u8 pr = 0; pr < 8; pr++)
	{
		if (process_context[pr].state == PROCESS_ACTIVE && process_context[pr].process_id == id)
		{
			return_value = ERR_IDU;
			break;
		}
	}

	if (return_value == ERR_IDU)
	{
		id = get_new_id(id);
	}

	u8 shifts = 0;
	for (u8 mask = 1 << 7; mask > 0; mask >>= 1, shifts++)
	{
		if (!(mask & active_processes))
		{
			active_processes |= mask;
			break;
		}
	}

	current_process = shifts;
	process *active_process = &process_context[current_process];

	active_process->state = PROCESS_ACTIVE;
	active_process->process_id = id;
	strncpy(active_process->process_name, process_name, 128);
	active_process->time_slice = priority;

	return return_value;
}

void schedule(void)
{
	// uart_printf((const u8 *)"Process runtime:%u\n",process_runtime);
	if (active_processes == 0)
	{
		write_csr_sepc(kernel_rpc);
		return;
	}

	if (process_runtime == process_context[current_process].time_slice)
	{
		uart_prints((const u8 *)"Current process time slice done!\n");
		process_runtime = 0;

		uart_printf((const u8 *)"SEPC before save:%x\n",read_csr_sepc());
		save_context(&process_context[current_process]);
		uart_printf((const u8 *)"SEPC after  save:%x\n",read_csr_sepc());

		uart_prints((const u8 *)"Saved current process context!\n");
		


		if (current_process < 7 && ((1 << (6 - current_process)) & active_processes))
		{
			current_process++;
		}
		else
		{
			current_process = 0;
		}

		uart_printf((const u8 *)"Next process index:%u\n", current_process);

		uart_printf((const u8 *)"SEPC before load:%x\n",read_csr_sepc());
		load_context(&process_context[current_process]);
		uart_printf((const u8 *)"SEPC after  load:%x\n",read_csr_sepc());

		uart_prints((const u8 *)"Loaded next process context!\n");
		uart_printf((const u8 *)"SEPC after  last print:%x\n",read_csr_sepc());
		return;
	}
	else
	{
		if (process_finished_running)
		{
			process_finished_running = 0;

			if (current_process == 8) // if there are no more active processes after one process has ninished running then return and wait for the next scheduling where control will be retrieved to kmain loop
			{
				return;
			}

			load_context(&process_context[current_process]);
			write_csr_sepc(process_context[current_process].env.pc);

			return;
		}

		process_runtime++;
	}
}

u8 save_context(process *active_process)
{
	if (active_process->state == PROCESS_INACTIVE)
	{
		return ERR_PRI;
	}

	u32 *caller_process_stack_frame = 0; // literally black magic
	__asm__ volatile("mv %0, s0"
					 : "=r"(caller_process_stack_frame)
					 :
					 :);

	caller_process_stack_frame = (u32 *)(*(caller_process_stack_frame - 2));

	// -0x50 needed for the actual sp value after saving temp registers (ra,s0,a0-a7,t0-t6)
	__asm__ volatile("lw %0,-0x4(%9) ;"
					 "addi %1,%9,-0x50 ;"
					 "mv %2,gp ;"
					 "mv %3,tp ;"
					 "lw %4,-0x8(%9) ;"
					 "lw %5,-0xc(%9) ;"
					 "lw %6,-0x10(%9) ;"
					 "lw %7,-0x14(%9) ;"
					 "mv %8,s1 ;"

					 : "=r"(process_context[current_process].env.x[ra]),
					   "=r"(process_context[current_process].env.x[sp]),
					   "=r"(process_context[current_process].env.x[gp]),
					   "=r"(process_context[current_process].env.x[tp]),
					   "=r"(process_context[current_process].env.x[t0]), "=r"(process_context[current_process].env.x[t1]), "=r"(process_context[current_process].env.x[t2]),
					   "=r"(process_context[current_process].env.x[s0]), "=r"(process_context[current_process].env.x[s1])
					 : "r"(caller_process_stack_frame)
					 :);

	__asm__ volatile(
		"lw %0,-0x18(%9) ;"
		"lw %1,-0x1c(%9) ;"
		"lw %2,-0x20(%9) ;"
		"lw %3,-0x24(%9) ;"
		"lw %4,-0x28(%9) ;"
		"lw %5,-0x2c(%9) ;"
		"lw %6,-0x30(%9) ;"
		"lw %7,-0x34(%9) ;"
		"mv %8,s2 ;"

		: "=r"(process_context[current_process].env.x[a0]), "=r"(process_context[current_process].env.x[a1]), "=r"(process_context[current_process].env.x[a2]), "=r"(process_context[current_process].env.x[a3]), "=r"(process_context[current_process].env.x[a4]), "=r"(process_context[current_process].env.x[a5]), "=r"(process_context[current_process].env.x[a6]), "=r"(process_context[current_process].env.x[a7]),
		  "=r"(process_context[current_process].env.x[s2])
		: "r"(caller_process_stack_frame)
		:);

	__asm__ volatile(
		"mv %0,s5 ;"
		"mv %1,s6 ;"
		"mv %2,s7 ;"
		"mv %3,s8 ;"
		"mv %4,s9 ;"
		"mv %5,s10 ;"
		"mv %6,s11 ;"
		"mv %7,s3 ;"
		"mv %8,s4 ;"

		: "=r"(process_context[current_process].env.x[s5]), "=r"(process_context[current_process].env.x[s6]), "=r"(process_context[current_process].env.x[s7]), "=r"(process_context[current_process].env.x[s8]), "=r"(process_context[current_process].env.x[s9]), "=r"(process_context[current_process].env.x[s10]), "=r"(process_context[current_process].env.x[s11]),
		  "=r"(process_context[current_process].env.x[s3]), "=r"(process_context[current_process].env.x[s4])
		:);

	__asm__ volatile(
		"lw %0,-0x40(%4) ;"
		"lw %1,-0x44(%4) ;"
		"lw %2,-0x38(%4) ;"
		"lw %3,-0x3c(%4) ;"

		: "=r"(process_context[current_process].env.x[t5]), "=r"(process_context[current_process].env.x[t6]),
		  "=r"(process_context[current_process].env.x[t3]), "=r"(process_context[current_process].env.x[t4])
		: "r"(caller_process_stack_frame)
		:);

	process_context[current_process].env.pc = read_csr_sepc();

	return 0;
}

u8 load_context(process *active_process)
{
	if (active_process->state == PROCESS_INACTIVE)
	{
		return ERR_PRI;
	}

	__asm__ volatile("mv sp,%0 ;"
					 "mv gp,%1 ;"
					 "mv tp,%2 ;"
					 "mv s1,%3 ;"
					 "mv s2,%4 ;"
		             "mv s3,%5 ;"
					 "mv s4,%6 ;"
					 "mv s5,%7 ;"
					 "mv s6,%8 ;"
					 "mv s7,%9 ;"
					 :
					 : "r"(process_context[current_process].env.x[sp]),
					   "r"(process_context[current_process].env.x[gp]),
					   "r"(process_context[current_process].env.x[tp]),
					   "r"(process_context[current_process].env.x[s1]), "r"(process_context[current_process].env.x[s2]), 
					   "r"(process_context[current_process].env.x[s3]), "r"(process_context[current_process].env.x[s4]), 
					   "r"(process_context[current_process].env.x[s5]), "r"(process_context[current_process].env.x[s6]), 
					   "r"(process_context[current_process].env.x[s7]) 
					 :);

	__asm__ volatile(
		"mv s8,%0 ;"
		"mv s9,%1 ;"
		"mv s10,%2 ;"
		"mv s11,%3 ;"
		:
		: "r"(process_context[current_process].env.x[s8]), "r"(process_context[current_process].env.x[s9]), 
		  "r"(process_context[current_process].env.x[s10]), "r"(process_context[current_process].env.x[s11])
		:);

	
	

	write_csr_sepc(process_context[current_process].env.pc);

	return 0;
}
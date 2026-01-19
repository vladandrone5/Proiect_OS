#include "process.h"
#include "../string/string.h"
#include "../misc/csr.h"
#include "../uart/uart.h"
#include "../memory/memory.h"

u8 process_finished_running = 0;
u8 last_used_id = 0;
u8 active_processes = 0;
u8 current_process = 0;
u8 process_runtime = 0;
 __attribute__((section(".proc_ctxs"))) process process_context[8];
process *process_list[8];
u32 kernel_rpc = 0;

u8 get_new_id(u8 id) /* gets new id if the curretn one is used */
{
	id+=(id==0);

	u8 new_id_used = 0;
	for (u8 new_id = 1; new_id < 256; new_id++)
	{
		new_id_used = 0;
		for (u8 pr = 0; pr < 8; pr++)
		{
			if ((process_list[pr]->state != PROCESS_DEAD) && process_list[pr]->process_id == id)
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

void start_process(void)
{
	write_csr_sepc(process_list[current_process]->env.pc);

	__asm__ volatile("sret");
}

void change_cp_on_kill(u8 p_ctx_idx) // process pointer index
{
	if(p_ctx_idx < current_process)
	{
		--current_process;
	}
	else if(p_ctx_idx == current_process)
	{
		if(process_list[current_process] == 0)
		{
			current_process = 0;
		}
	}
}

void initialize_processes(void)
{
	for (u8 pr = 0; pr < 8; pr++)
	{
		process_list[pr] = 0;

		process_context[pr].process_id = 0;
		process_context[pr].time_slice = 0;
		process_context[pr].state = PROCESS_DEAD;
		strncpy(process_context[pr].process_name, (const u8 *)"", 1);

		for (u8 reg_i = 0; reg_i < 32; reg_i++)
		{
			process_context[pr].env.x[reg_i] = 0;
		}

		process_context[pr].env.x[sp] = (u32)&process_context[pr].stack[PROCESS_STACK_SIZE];

		for(u32 sp_idx = 0; sp_idx < PROCESS_STACK_SIZE; sp_idx++)
		{
			process_context[pr].stack[sp_idx] = 0;
		}
	}
}

void process_done(void)
{
	// TO DO: call remove process here
	process_finished_running = 1;

	__asm__ volatile("wfi");
}

u8 add_process(u8 id, u8 priority, const u8 *process_name, u32 program_location)
{
	// check if id used(shouldnt be), if there is enough space for a new process
	if (active_processes == 255)
	{
		return ERR_PBF;
	}

	u8 return_value = 0;
	for (u8 pr = 0; pr < 8; pr++)
	{
		if(process_list[pr] == 0)
		{
			break;
		}

		if ((process_list[pr]->state != PROCESS_DEAD ) && process_list[pr]->process_id == id)
		{
			return_value = ERR_IDU;
			break;
		}
	}

	if(return_value == ERR_IDU)
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

	process *active_process = 0;
	for(u8 pr = 0; pr < 8; pr++)
	{
		if(process_context[pr].state == PROCESS_DEAD)
		{
			active_process = &process_context[pr];
			break;
		}
	}

	process_list[shifts] = active_process;

	active_process->state = PROCESS_INACTIVE;
	active_process->process_id = id;
	strncpy(active_process->process_name, process_name, 128);
	active_process->time_slice = priority;
	active_process->env.pc = program_location;

	memset(active_process->stack,PROCESS_STACK_SIZE,0);
	active_process->stack[PROCESS_STACK_SIZE-16] = (u32)start_process;
	active_process->env.x[sp] = (u32)&active_process->stack[PROCESS_STACK_SIZE-16]; // 13 represents stack frame for context switching

	return return_value;
}

void schedule(void)
{   
	// refactor function after project is done so that it uses assembly function for switching context
	if (active_processes == 0)
	{
		write_csr_sepc(kernel_rpc);
		return;
	}

	u8 new_process = current_process + 1;
	process *curr_proc = process_list[current_process];

	if(process_runtime >= curr_proc->time_slice)
	{
		process_runtime = 0;

		if(new_process>7 || process_list[new_process] == 0)
		{
			new_process = 0;
		}

		if((current_process == new_process) && curr_proc->state == PROCESS_ACTIVE)
		{
			return;
		}

		process *next_proc = process_list[new_process];

		
		if(curr_proc->state == PROCESS_ACTIVE)
		{
			curr_proc->state = PROCESS_WAITING;
			curr_proc->env.pc = read_csr_sepc();
			next_proc->state = PROCESS_ACTIVE;

			current_process = new_process;

			write_csr_sepc(next_proc->env.pc);
			switch_context(curr_proc, next_proc);

			return;
		}
		
		curr_proc->state = PROCESS_ACTIVE;
		write_csr_sepc(curr_proc->env.pc);
		load_context(curr_proc);
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

			return;
		}

		process_runtime++;
	}
}

u8 remove_process(u8 p_ctx_idx)
{
	if(active_processes == 0)
	{
		return 0;
	}

	process_runtime = 255;


	active_processes<<=1;

	// other removel stuff
	process *active_process = process_list[p_ctx_idx];
	active_process->state = PROCESS_DEAD;
	active_process->process_id = 0;
	strncpy(active_process->process_name,(const u8 *)"",1);

	for(u8 pr = p_ctx_idx; pr < 7; pr++)
	{
		process_list[pr] = process_list[pr+1];
	}

	process_list[7] = 0;

	change_cp_on_kill(p_ctx_idx);

	return 0;
}
#include "process.h"
#include "../string/string.h"
#include "../misc/csr.h"

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
  //TO DO: call remove process here
  process_finished_running = 1;

  __asm__ volatile("wfi");
}

u8 add_process(u8 id,u8 priority,const u8 *process_name)
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
  if(active_processes == 0)
  {
    write_csr_sepc(kernel_rpc);
    return;
  }
  
  if(process_runtime == process_context[current_process].time_slice)
  {
    process_runtime=0;
    save_context(&process_context[current_process]);
    
    if(current_process<7 && ((1<<(6-current_process)) & active_processes))
    {
      current_process++;
    }
    else
    {
      current_process=0;
    }

    load_context(&process_context[current_process]);
    write_csr_sepc(process_context[current_process].env.pc);
  }
  else
  {
    if(process_finished_running)
    {
      process_finished_running = 0;
      
      if(current_process == 8) // if there are no more active processes after one process has ninished running then return and wait for the next scheduling where control will be retrieved to kmain loop
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

  __asm__ volatile("mv %0,zero ;"
                   "mv %1,ra ;"
                   "mv %2,sp ;"
                   "mv %3,gp ;"
                   "mv %4,tp ;"
                   "mv %5,t0 ;"
                   "mv %6,t1 ;"
                   "mv %7,t2 ;"
                   "mv %8,s0 ;"
                   "mv %9,s1 ;"

                   : "=r"(process_context[current_process].env.x[zero]),
                     "=r"(process_context[current_process].env.x[ra]),
                     "=r"(process_context[current_process].env.x[sp]),
                     "=r"(process_context[current_process].env.x[gp]),
                     "=r"(process_context[current_process].env.x[tp]),
                     "=r"(process_context[current_process].env.x[t0]), "=r"(process_context[current_process].env.x[t1]), "=r"(process_context[current_process].env.x[t2]),
                     "=r"(process_context[current_process].env.x[s0]), "=r"(process_context[current_process].env.x[s1])
                   :
                   :);

  __asm__ volatile(
      "mv %0,a0 ;"
      "mv %1,a1 ;"
      "mv %2,a2 ;"
      "mv %3,a3 ;"
      "mv %4,a4 ;"
      "mv %5,a5 ;"
      "mv %6,a6 ;"
      "mv %7,a7 ;"
      "mv %8,s2 ;"
      "mv %9,s3 ;"

      : "=r"(process_context[current_process].env.x[a0]), "=r"(process_context[current_process].env.x[a1]), "=r"(process_context[current_process].env.x[a2]), "=r"(process_context[current_process].env.x[a3]), "=r"(process_context[current_process].env.x[a4]), "=r"(process_context[current_process].env.x[a5]), "=r"(process_context[current_process].env.x[a6]), "=r"(process_context[current_process].env.x[a7]),
        "=r"(process_context[current_process].env.x[s2]), "=r"(process_context[current_process].env.x[s3])
      :
      :);

  __asm__ volatile(
      "mv %0,s4 ;"
      "mv %1,s5 ;"
      "mv %2,s6 ;"
      "mv %3,s7 ;"
      "mv %4,s8 ;"
      "mv %5,s9 ;"
      "mv %6,s10 ;"
      "mv %7,s11 ;"
      "mv %8,t3 ;"
      "mv %9,t4 ;"

      : "=r"(process_context[current_process].env.x[s4]), "=r"(process_context[current_process].env.x[s5]), "=r"(process_context[current_process].env.x[s6]), "=r"(process_context[current_process].env.x[s7]), "=r"(process_context[current_process].env.x[s8]), "=r"(process_context[current_process].env.x[s9]), "=r"(process_context[current_process].env.x[s10]), "=r"(process_context[current_process].env.x[s11]),
        "=r"(process_context[current_process].env.x[t3]), "=r"(process_context[current_process].env.x[t4])
      :
      :);

  __asm__ volatile(
      "mv %0,t5 ;"
      "mv %1,t6 ;"

      : "=r"(process_context[current_process].env.x[t5]), "=r"(process_context[current_process].env.x[t6])
      :
      :);

  __asm__ volatile(
    "auipc %0,0 ;"
    :"=r"(process_context[current_process].env.pc)
    :
    :);

  return 0;
}

u8 load_context(process *active_process)
{
  if (active_process->state == PROCESS_INACTIVE)
  {
    return ERR_PRI;
  }

  // check bit apparition mismatch

  __asm__ volatile("mv zero,%0 ;"
                   "mv ra,%1 ;"
                   "mv sp,%2 ;"
                   "mv gp,%3 ;"
                   "mv tp,%4 ;"
                   "mv t0,%5 ;"
                   "mv t1,%6 ;"
                   "mv t2,%7 ;"
                   "mv s0,%8 ;"
                   "mv s1,%9 ;"

                   :
                   : "r"(process_context[current_process].env.x[zero]),
                     "r"(process_context[current_process].env.x[ra]),
                     "r"(process_context[current_process].env.x[sp]),
                     "r"(process_context[current_process].env.x[gp]),
                     "r"(process_context[current_process].env.x[tp]),
                     "r"(process_context[current_process].env.x[t0]), "r"(process_context[current_process].env.x[t1]), "r"(process_context[current_process].env.x[t2]),
                     "r"(process_context[current_process].env.x[s0]), "r"(process_context[current_process].env.x[s1])
                   :);

  __asm__ volatile(
      "mv a0,%0 ;"
      "mv a1,%1 ;"
      "mv a2,%2 ;"
      "mv a3,%3 ;"
      "mv a4,%4 ;"
      "mv a5,%5 ;"
      "mv a6,%6 ;"
      "mv a7,%7 ;"
      "mv s2,%8 ;"
      "mv s3,%9 ;"
      :
      : "r"(process_context[current_process].env.x[a0]), "r"(process_context[current_process].env.x[a1]), "r"(process_context[current_process].env.x[a2]), "r"(process_context[current_process].env.x[a3]), "r"(process_context[current_process].env.x[a4]), "r"(process_context[current_process].env.x[a5]), "r"(process_context[current_process].env.x[a6]), "r"(process_context[current_process].env.x[a7]),
        "r"(process_context[current_process].env.x[s2]), "r"(process_context[current_process].env.x[s3])
      :);

  __asm__ volatile(
      "mv s4,%0 ;"
      "mv s5,%1 ;"
      "mv s6,%2 ;"
      "mv s7,%3 ;"
      "mv s8,%4 ;"
      "mv s9,%5 ;"
      "mv s10,%6 ;"
      "mv s11,%7 ;"
      "mv t3,%8 ;"
      "mv t4,%9 ;"
      :
      : "r"(process_context[current_process].env.x[s4]), "r"(process_context[current_process].env.x[s5]), "r"(process_context[current_process].env.x[s6]), "r"(process_context[current_process].env.x[s7]), "r"(process_context[current_process].env.x[s8]), "r"(process_context[current_process].env.x[s9]), "r"(process_context[current_process].env.x[s10]), "r"(process_context[current_process].env.x[s11]),
        "r"(process_context[current_process].env.x[t3]), "r"(process_context[current_process].env.x[t4])
      :);

  __asm__ volatile(
      "mv t5,%0 ;"
      "mv t6,%1 ;"
      :
      : "r"(process_context[current_process].env.x[t5]), "r"(process_context[current_process].env.x[t6])
      :);

  // __asm__ volatile(
  //   "mv pc,%0 ;"
  //   :
  //   :"r"(process_context[current_process].env.pc)
  // : );
  // loading pc is not needed as sepc will become pc

  return 0;
}
#ifndef PROCESS_H
#define PROCESS_H

#include "../misc/types.h"

#define ERR_PRI 1 // process inactive
#define ERR_PBF 2 // no more space to create process (process buffer full)
#define ERR_IDU 3 // id already used

typedef enum
{
    zero,
    ra,
    sp,
    gp,
    tp,
    t0,t1,t2,
    s0,s1,
    a0,a1,a2,a3,a4,a5,a6,a7,
    s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,
    t3,t4,t5,t6
}REGISTER_ABI_NAME;

typedef enum
{ // MAYBE ADD PROCESS WAITING AND MAYBE PROCESS DEAD
    PROCESS_INACTIVE,
    PROCESS_ACTIVE
}PROCESS_STATE;

typedef struct
{
    u32 x[32]; 
    u32 pc;
}regs;

typedef struct process_t
{
    regs env; 
    u8 process_name[128];
    PROCESS_STATE state;
    u8 process_id;
    u8 time_slice;
}process;

extern u8 last_used_id ;
extern u8 active_processes; /* bitwise apparition vector to know how many processes are active */
extern u8 current_process; /* numeric index to current process that runs */
extern u8 process_runtime; /* used to await priority (n) timer interrupts for each process */
extern process process_context[8]; /* hold data for all active processes */
extern u32 kernel_rpc;

u8 get_new_id(u8 id);
void initialize_processes(void);
void process_done(void);
u8 save_context(process *active_process);
u8 load_context(process *active_process);
u8 add_process(u8 id,u8 priority,const u8 *process_name);
u8 remove_process(process *active_process);
void schedule(void);

#endif
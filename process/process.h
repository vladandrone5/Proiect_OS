#ifndef PROCESS_H
#define PROCESS_H

#include "../misc/types.h"

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
{
    PROCESS_INACTIVE,
    PROCESS_ACTIVE
}PROCESS_STATE;

typedef struct
{
    u32 x[32]; 
}regs;

typedef struct process_t
{
    PROCESS_STATE state;
    u8 process_id;
    u8 process_idx;
    u8 priority_lvl;
    regs process_env; 
}process;

extern u8 active_processes;
extern u8 current_process;
extern u8 process_runtime[8];
extern process process_context[8];

void save_context(process *active_process);
void load_context(process *active_process);
u8 add_process(process *active_process,u8 id, u8 priority);
u8 remove_process(proces *active_process);
void schedule(void);

#endif
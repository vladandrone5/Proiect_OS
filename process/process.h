#ifndef PROCESS_H
#define PROCESS_H

#include "../misc/types.h"

#define ERR_PRI 1 // process inactive
#define ERR_PBF 2 // no more space to create process (process buffer full)
#define ERR_IDU 3 // id already used

#define PROCESS_STACK_SIZE (u32)(1000000/4)

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
    PROCESS_ACTIVE,
    PROCESS_WAITING,
    PROCESS_DEAD
}PROCESS_STATE;

typedef struct
{
    u32 x[32]; 
    u32 pc;
}regs;

typedef struct process_t
{
    regs env; 
    u32 stack[PROCESS_STACK_SIZE];
    u8 process_name[128];
    u8 process_id;
    u8 time_slice;
    PROCESS_STATE state;
   
}process;

extern u8 last_used_id ;
extern u8 active_processes; /* bitwise apparition vector to know how many processes are active */
extern u8 current_process; /* numeric index to current process that runs */
extern u8 process_runtime; /* used to await priority (n) timer interrupts for each process */
extern process process_context[8]; /* hold data for all active processes */
extern process *process_list[8]; /* pointers to processes, used for process reordering->contiguous line of bits representing running programs */
extern u32 kernel_rpc;

extern void switch_context(process *old_process, process *new_process);
extern void load_context(process *new_process);

u8 make_mask_portion(u8 no_bits);
u8 get_new_id(u8 id);
void start_process(void);
void change_cp_on_kill(u8 p_ctx_idx);
void initialize_processes(void);
void process_done(void);
u8 add_process(u8 id,u8 priority,const u8 *process_name,u32 program_location);
u8 remove_process(u8 p_ctx_idx);
void schedule(void);

#endif
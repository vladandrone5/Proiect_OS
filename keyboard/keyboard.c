#include "keyboard.h"
#include "../plic/plic.h"

void setup_keyboard_interrupt(u16 source,u32 priority, u32 ctx_addr)
{
    set_plic_source_pry(source, priority);
    set_plic_source_int_en(ctx_addr, source);
}
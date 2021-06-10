#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "../x86/int/isr.h"

void scheduler_yield(struct regs *regs);
void scheduler_init(struct regs *kregs);

#endif
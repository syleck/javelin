#ifndef SYSCALL_H
#define SYSCALL_H
#include "../x86/int/isr.h"

int do_syscall(struct regs* r);

#endif
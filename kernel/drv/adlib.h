#ifndef ADLIB_H
#define ADLIB_H
#include "../x86/asm.h"
#define WREG(REG,DAT) outb(0x388,REG); outb(0x389,DAT);

int init_adlib();

#endif
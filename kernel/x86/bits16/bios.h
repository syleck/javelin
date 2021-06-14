#ifndef BIOS_H
#define BIOS_H

#include <stdint.h>

struct int86regs {
    uint32_t edi, esi, ebp, esp;
    uint32_t ebx, edx, ecx, eax;
    uint16_t flags;
    uint16_t es, ds, fs, gs;
} __attribute__ ((packed));

void int86(int inum, struct int86regs *regs);

#endif
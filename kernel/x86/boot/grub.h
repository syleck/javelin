#ifndef GRUB_H
#define GRUB_H
#include <stdint.h>

void multiboot_init(uint32_t eax, uint32_t ebx);
char* get_symbol(void* addr);

#endif
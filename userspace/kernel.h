#ifndef __USERSPACE_KERNEL_H__
#define __USERSPACE_KERNEL_H__
#include <stdarg.h>

// Systems for running with the kernel

#define KERNEL_INTERRUPT 0x80

/**
 * @brief Printf (Userspace edition)
 * 
 * @param fmt Format of string
 * @param ... Arguments to put in format
 */
static void printf(const char* fmt,...) {
    va_list ap;
    va_start(ap,fmt);
    __asm__ volatile ("mov  1,%%eax\n" // call function 1
                      "mov %0,%%ebx\n" // mov format to ebx
                      "mov %1,%%ecx\n" // mov arguments to ecx
                      "int %2\n"       // interrupt system
    : : "m"(fmt), "r"(ap), "N"(KERNEL_INTERRUPT) : "eax", "ebx", "ecx");
    va_end(ap);
}

#endif
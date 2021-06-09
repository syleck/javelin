#ifndef V86_H
#define V86_H
#include <stdint.h>

// see /kernel/x86/bits16/v86.asm
typedef enum {
    unknown,
    v86_hires_textmode
} v86_subroutine;

void start_v86_sub(v86_subroutine sub);
void init_v86();

#endif
#ifndef TTY_H
#define TTY_H
#include "stdio.h"
#include <stdint.h>


extern int terminal_row;
extern int terminal_col;

void init_tty();
void add_echo_tty(io_struct* str);
void update_cursor(int x, int y);
void tty_setcolor(uint8_t c);
void tty_refresh();

#endif
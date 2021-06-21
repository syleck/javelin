#ifndef TTY_H
#define TTY_H
#include "stdio.h"
#include <stdint.h>


extern int terminal_row;
extern int terminal_col;

#define TERM_COLOR 0x87
#define TERM_SCLOR 0x9F
#define CONSOLE_HEIGHT 48
#define CONSOLE_WIDTH 128
#define VGAVIDEO_PTR  0x00000
#define VIDEO_POINTER 0xfd000000

void init_tty();
void add_echo_tty(io_struct* str);
void update_cursor(int x, int y);
void tty_setcolor(uint8_t c);
void tty_refresh();

#endif
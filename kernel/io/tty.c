#include "tty.h"
#include "../drv/device.h"
#include "../module.h"
#include "../stdlib.h"
#include "../x86/asm.h"
#include "serial.h"
#include "../sys/framebuffer.h"
#include <stdbool.h>

// TODO: Make this better and make it work with the framebuffer more better

MODULE("TTY");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

bool text_mode = true;
bool term_init = false;
#define CHKTERM() if(!term_init) return;

int terminal_row = 0;
int terminal_col = 0;

#define VIDEO_PTRFROMXY(x,y) \
    ((uint16_t*)VIDEO_POINTER)[x + 80 * y]

io_struct* echo_ttys[64];
int echo_ttyc = 0;
uint8_t term_color = TERM_COLOR;

void tty_scroll() {
    /*
    CHKTERM()
    terminal_row = CONSOLE_HEIGHT-1;
    uint16_t terminal_buffer[(CONSOLE_HEIGHT*CONSOLE_WIDTH)+CONSOLE_WIDTH];
    memcpy(terminal_buffer,VIDEO_POINTER,(CONSOLE_HEIGHT*CONSOLE_WIDTH)*2);
    //memset(VIDEO_POINTER,'\0',(CONSOLE_HEIGHT*CONSOLE_WIDTH)*2);
    int orow = terminal_row;
    int ocol = terminal_col;
    tty_clear(); // this sets row and col back to its default positions, dont want that
    terminal_row = orow;
    terminal_col = ocol;
    memcpy(VIDEO_POINTER-(CONSOLE_WIDTH*2),terminal_buffer,(CONSOLE_HEIGHT*CONSOLE_WIDTH)*2);
    if(text_mode) {
        for(int i = 0; i < CONSOLE_WIDTH; i++) {
            // clear the scheduler line
            *((uint16_t*)VIDEO_POINTER+i) = TERM_SCLOR<<8;
        }
    }
    */
}

void tty_setcolor(uint8_t c) {
    term_color = c;
}

void tty_clear() {
    /*
    CHKTERM()
    if(text_mode) {
        for(int i = 0; i < (CONSOLE_HEIGHT*CONSOLE_WIDTH)*2; i++) {
            *((uint16_t*)VIDEO_POINTER+i) = term_color<<8;
        }
        for(int i = 0; i < CONSOLE_WIDTH; i++) {
            // clear the scheduler line
            *((uint16_t*)VIDEO_POINTER+i) = TERM_SCLOR<<8;
        }
    }*/
    terminal_row = 0;
    terminal_col = 0;
}

void tty_putch(char i) {
    CHKTERM()
    if(i == '\n') {
        terminal_row++;
        terminal_col = 0;
        if(terminal_row != 0)   
            write_serial('\r');
            write_serial('\n');
        return;
    }
    uint16_t ch;
    uint16_t cha = (uint16_t)i;
    ch = cha;
    ch |= term_color << 8;
    if(terminal_row >= CONSOLE_HEIGHT) {
        tty_scroll();
    }
    //VIDEO_PTRFROMXY(terminal_col,terminal_row) = ch;
    drawchar(i,terminal_col*CHAR_WIDTH,terminal_row*CHAR_HEIGHT,0xffffff,0x606060);
    if(terminal_row != 0)
        write_serial(i);
    for(int i = 0; i < echo_ttyc; i++) {
        echo_ttys[i]->write_byte(i);
    }
    if(terminal_col++ == CONSOLE_WIDTH) {
        terminal_col = 0;
        if(terminal_row++ == CONSOLE_HEIGHT) {
            terminal_row = 0;
        }
    }
}

void tty_putstr(char* i, int l) {
    int j = 0;
    while(l != 0) {
        tty_putch(*(i+j));
        j++; l--;
    }
    update_cursor(terminal_col,terminal_row);
}

io_struct tty = {
    .write_byte = tty_putch,
    .write_stream = tty_putstr,
    .cmode = IO_INPUT,
};

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
    /*
    CHKTERM()
    if(text_mode) {
        outb(0x3D4, 0x0A);
        outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
    
        outb(0x3D4, 0x0B);
        outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
    }
    */
}

void disable_cursor()
{
    /*
    CHKTERM()
    if(text_mode) {
        outb(0x3D4, 0x0A);
        outb(0x3D5, 0x20);
    }*/
}

void update_cursor(int x, int y)
{
    /*
    CHKTERM()
	uint16_t pos = y * CONSOLE_WIDTH + x;
 
    if(text_mode) {
        outb(0x3D4, 0x0F);
        outb(0x3D5, (uint8_t) (pos & 0xFF));
        outb(0x3D4, 0x0E);
        outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
    }
    */
    terminal_col = x;
    terminal_row = y;
}

void add_echo_tty(io_struct* str) {
    CHKTERM()
    DVERBOSE(mprintf("New echo tty: %x\n",str));
    echo_ttys[echo_ttyc] = str;
    echo_ttyc++;
}

void init_tty() {
    term_init = true;
    tty_handle = &tty;
    tty_clear();
    int d = add_simple_text("tty0",tty);
    add_alias(get_device(d),"tty");
    terminal_row = 1;
}

void tty_refresh() {
    //memcpy(TRUE_VIDEO_POINTER,VIDEO_POINTER,(CONSOLE_HEIGHT*CONSOLE_WIDTH)*2);
}
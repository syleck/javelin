#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <stdint.h>

extern int c_res_x;
extern int c_res_y;
extern uint32_t* framebuffer;
#define CHAR_HEIGHT 8
#define CHAR_WIDTH 8

void fb_init();
void putpixel(unsigned char* screen, int x,int y, int color);
void drawchar(unsigned char c, int x, int y, int fgcolor, int bgcolor);
void fillrect(unsigned char *vram, int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char w, unsigned char h);
void fbupdate();

#endif
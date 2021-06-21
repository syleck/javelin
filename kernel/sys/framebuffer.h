#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

void fb_init();
void putpixel(unsigned char* screen, int x,int y, int color);
void drawchar(unsigned char c, int x, int y, int fgcolor, int bgcolor);
void fillrect(unsigned char *vram, unsigned char r, unsigned char g, unsigned char b, unsigned char w, unsigned char h);
void fbupdate();

#endif
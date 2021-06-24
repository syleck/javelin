#include "wm.h"
#include "../framebuffer.h"
#include "../../string.h"
#include <stddef.h>

window *windows[1024];
window_toolkit toolkit;
struct {
    int text_padding;
} system_style;
int windowc = 0;
int repaint_tries = 0;

// wm toolkit

void wtk_drawtext(window* window, int x, int y, char* text) {
    int p = 0;
    while(*text!=0) {
        drawchar(*text,window->x+x+p,window->y+y,0x000000,0xffffff);
        text++;
        if(p > window->w) {
            p = 0;
        }
        p += CHAR_WIDTH + system_style.text_padding;
    }
}

// helper to do it without wtk
void drawtext(int x, int y, char* text) {
    int p = 0;
    while(*text!=0) {
        drawchar(*text,x+p,y,0x000000,0xffffff);
        text++;
        p += CHAR_WIDTH + system_style.text_padding;
    }
}

void wtk_putpixel(window* window, int x, int y, int c) {
    putpixel(framebuffer,window->x+x,window->y+y,c);
}

// about box

void about_repaint(window* win) {
    wtk_drawtext(win,10,10,"WM 1.0");
    wtk_drawtext(win,10,28,"for Javelin");
    char repaints[255];
    itoa(repaint_tries,16,repaints);
    wtk_drawtext(win,10,38,repaints);
}

window* create_about() {
    window* win = kmalloc(sizeof(window),"AboutBox");
    win->ownerpid = 1;
    win->repaint = about_repaint;
    win->h = 30;
    win->w = 10;
    win->ox = win->x = 50;
    win->oy = win->y = 50;
    win->visible = true;
    win->title = "About WM";
    return win;
}

// actual wm code

void init_wm() {
    toolkit.drawtext = wtk_drawtext;
    toolkit.putpixel = wtk_putpixel;

    system_style.text_padding = 0;
    
    new_window(create_about());
}

void new_window(window* win) {
    printf("New window: %s, at %x\n",win->title,win);
    windows[windowc] = win;
    win->h *= CHAR_HEIGHT;
    win->w *= CHAR_WIDTH;
    windowc++;
}

void repaint_all() {
    #ifndef ONT_REPAINT
    drawtext(0,0,"WM1.0/WTK1.0 ");
    int tasklc = 13*CHAR_WIDTH;
    for(int i = 0; i < windowc; i++) {
        if(windows[i]->visible == true) {
            drawtext(tasklc,0,windows[i]->title);
            tasklc += (strlen(windows[i]->title)+1)*CHAR_WIDTH;
            drawtext(tasklc-CHAR_WIDTH,0," ");
            windows[i]->toolkit = &toolkit;

            if(!windows[i]->drawnonce) {
                fillrect(framebuffer,windows[i]->x-3,windows[i]->y-9,0xfa,0xcc,0xcc,windows[i]->w+10,windows[i]->h+6);
                fillrect(framebuffer,windows[i]->x,windows[i]->y,0xff,0xff,0xff,windows[i]->w,windows[i]->h);
                windows[i]->drawnonce = true;
            }
            if(windows[i]->ox != windows[i]->x || windows[i]->oy != windows[i]->y) {
                fillrect(framebuffer,windows[i]->ox,windows[i]->oy,0x00,0x00,0x00,windows[i]->w,windows[i]->h);
                windows[i]->ox = windows[i]->x;
                windows[i]->oy = windows[i]->y;
                fillrect(framebuffer,windows[i]->x-3,windows[i]->y-9,0xfa,0xcc,0xcc,windows[i]->w+10,windows[i]->h+6);
                fillrect(framebuffer,windows[i]->x,windows[i]->y,0xff,0xff,0xff,windows[i]->w,windows[i]->h);
            }
            
            windows[i]->toolkit->drawtext(windows[i],0,-9,windows[i]->title);
            windows[i]->repaint(windows[i]);
        }
    }
    repaint_tries++;
    #endif
}
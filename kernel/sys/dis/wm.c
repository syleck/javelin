#include "wm.h"
#include "../framebuffer.h"
#include "../../string.h"
#include "../../module.h"
#include "../../mem.h"
#include "../state.h"
#include "term.h"
#include <stddef.h>

MODULE("WINDOWMANAGER")
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

window *windows[1024];
window_toolkit toolkit;
struct {
    int text_padding;
    int fgcolor;
    int bgcolor;
} system_style;
int windowc = 0;
int repaint_tries = 0;

// wm toolkit

void wtk_drawtext(window* window, int x, int y, char* text) {
    int p = 0;
    while(*text!=0) {
        window->toolkit->drawchar(window,x+p,y,*text);
        text++;
        if(p > window->w) {
            p = 0;
        }
        p += CHAR_WIDTH + system_style.text_padding;
    }
}

void wtk_drawchar(window* window, int x, int y, char text) {    
    int cx,cy;
    int mask[]={1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384};
    unsigned char *gylph=get_info(SYSINFO_TEXT_FBFONT_START)+(int)text*CHAR_HEIGHT;
    x+=CHAR_WIDTH-1;
    bool snc = false;

    if(system_style.bgcolor == 0) {
        snc = true;
    }

    for(cy=0;cy<CHAR_HEIGHT;cy++){
        for(cx=CHAR_WIDTH-1;cx>=0;cx--){
            putpixel(window->bitmap,x-cx,y+cy,gylph[cy]&mask[cx]?system_style.fgcolor:system_style.bgcolor);
        }
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
    toolkit.drawchar = wtk_drawchar;

    system_style.text_padding = 0;
    
    new_window(create_about());
    new_window(term_create());
}

void new_window(window* win) {
    printf("New window: %s, at %x\n",win->title,win);
    windows[windowc] = win;
    win->bitmap = malloc(win->w*win->h);
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
            toolkit.drawtext = wtk_drawtext;
            toolkit.drawchar = wtk_drawchar;
            toolkit.putpixel = wtk_putpixel;
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
            draw_icon(windows[i]->x,windows[i]->y,windows[i]->w,windows[i]->h,windows[i]->bitmap);
        }
    }
    repaint_tries++;
    #endif
}
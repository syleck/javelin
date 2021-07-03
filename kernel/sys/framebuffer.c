#include "framebuffer.h"
#include "state.h"
#include "ifnt/sysfont.h"
#include "dis/tga.h"
#include <stddef.h>
#include <stdbool.h>
#include "../x86/int/irq.h"
#include "../module.h"

MODULE("FRAMEBUFFER")
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

uint32_t* framebuffer;
int fblen;
int c_res_x = 0;
int c_res_y = 0;
int CONSOLE_WIDTH = 80;
int CONSOLE_HEIGHT = 25;
int c_bpp;
int c_pitch;
unsigned char *font;
uint32_t* tga;

void fb_init() {
    fbupdate();
}

void fb_clear() {
    for(int i = 0; i < c_res_x; i++) {
        for(int j = 0; j < c_res_y; j++) {
            putpixel(framebuffer,j,i,0x606060);
        }
    }
    //draw_icon(c_res_x-100,0,tga[0],tga[1],tga+2);
}
 
void putpixel(unsigned char* screen, int x, int y, int color) {
    unsigned where = x*4 + y*c_res_x;
    /*if(where > framebuffer+fblen)
        return;
    if(where < framebuffer)
        return;*/
    screen[where] = color & 255;              // BLUE
    screen[where + 1] = (color >> 8) & 255;   // GREEN
    screen[where + 2] = (color >> 16) & 255;  // RED
}

void fillrect(unsigned char *vram, int x, int y, unsigned char r, unsigned char g, unsigned   char b, unsigned char w, unsigned char h) {
    unsigned char *where = vram + x*4 + y*c_res_x;
    int i, j;
 
    for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
            //putpixel(vram, 64 + j, 64 + i, (r << 16) + (g << 8) + b);
            /*if(where > framebuffer+fblen)
                continue;
            if(where < framebuffer)
                continue;*/
            where[j*4] = r;
            where[j*4 + 1] = g;
            where[j*4 + 2] = b;
        }
        where+=c_res_x;
    }
}

void drawchar(unsigned char c, int x, int y, int fgcolor, int bgcolor)
{
	int cx,cy;
	int mask[]={1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384};
	unsigned char *gylph=font+(int)c*CHAR_HEIGHT;
    x+=CHAR_WIDTH-1;
    bool snc = false;

    if(bgcolor == 0) {
        snc = true;
    }

	for(cy=0;cy<CHAR_HEIGHT;cy++){
		for(cx=CHAR_WIDTH-1;cx>=0;cx--){
			putpixel(framebuffer,x-cx,y+cy,gylph[cy]&mask[cx]?fgcolor:bgcolor);
		}
	}
}

void drawstring(unsigned char* c, int x, int y, int fgcolor, int bgcolor) {
    int p = 0;
    while(*c!=0) {
        drawchar(*c,x+p,y,fgcolor,bgcolor);
        c++;
        p += CHAR_WIDTH;
    }
}

void draw_icon(int x, int y, int w, int h, int *pixels) {
    int j = 0;
    for (int l = 0; l < h; l++) {
        for (int i = 0; i < w; i++, j++) {
            putpixel(framebuffer,x + i, y + l, pixels[j]);
        }
    }
}

void fbupdate() {
    framebuffer = get_info(SYSINFO_VIDEO_ADDR);
    c_res_x = get_info(SYSINFO_VIDEO_RESX);
    c_res_y = get_info(SYSINFO_VIDEO_RESY);
    c_bpp = get_info(SYSINFO_VIDEO_BPP);
    c_pitch = get_info(SYSINFO_VIDEO_PITCH);
    fblen = c_res_x*c_res_y*c_bpp*c_pitch;
    tty_clear();
    font = vincent_data;
    if(get_info(SYSINFO_TEXT_FBFONT_START)) {
        font = get_info(SYSINFO_TEXT_FBFONT_START);
    }

    /*if(get_info(SYSINFO_ICON_START)) {
        tga_header_t* tga_header = get_info(SYSINFO_ICON_START);
        tga = tga_parse(tga_header+sizeof(tga_header_t),get_info(SYSINFO_ICON_END-SYSINFO_ICON_START));
        draw_icon(c_res_x-100,0,tga[0],tga[1],tga+2);
    }*/
    
    printf("%ix%ix%i\n",c_res_x,c_res_y,c_bpp);    
}
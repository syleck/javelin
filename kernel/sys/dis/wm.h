#ifndef WM_H
#define WM_H
#include <stdbool.h>
#include "../framebuffer.h"

// first void* arg should be window_t
typedef struct {
    // window, x, y, string
    void (*drawtext)(void*, int, int, char*);
    // window, x, y, color
    void (*putpixel)(void*, int, int, int);
    // window, x, y, length, width, data
    void (*drawicon)(void*, int, int, int, int, void*);
} window_toolkit;

typedef struct window_t {
    int x;
    int y;
    int ox;
    int oy;
    int w;
    int h;

    int ownerpid;
    char* title;
    bool visible;
    bool drawnonce;

    // provided by wm
    window_toolkit* toolkit;
    // window
    void (*repaint)(struct window_t*);
    // window, x, y, type
    void (*click)(struct window_t*, int, int, int);
} window;

void init_wm();
void new_window(window* win);
void repaint_all();

#endif
#ifndef WM_H
#define WM_H
#include <stdbool.h>

// first void* arg should be window_t
typedef struct {
    void (*drawtext)(void*, int, int, char*);
    void (*putpixel)(void*, int, int, int);
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
    void (*repaint)(struct window_t*);
} window;

void init_wm();
void new_window(window* win);
void repaint_all();

#endif
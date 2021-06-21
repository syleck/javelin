#ifndef WM_H
#define WM_H

// first void* arg should be window_t
typedef struct {
    void (*drawtext)(void*, int, int, char*);
    void (*drawicon)(void*, int, int, int, int, void*);
} window_toolkit;

typedef struct window_t {
    int x;
    int y;
    int w;
    int h;

    int ownerpid;
    char* title;

    // provided by wm
    window_toolkit* toolkit;
    void (*repaint)(struct window_t*);
} window;

void init_wm();
void new_window(window* win);
void repaint_all();

#endif
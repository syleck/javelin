#include "wm.h"
#include "../framebuffer.h"
#include <stddef.h>

window *windows[1024];
int windowc = 0;

void init_wm() {
    
}

void new_window(window* win) {
    windows[windowc++] = win;
}

void repaint_all() {
    for(int i = 0; i < windowc; i++) {
        if(windows[i]->ownerpid != NULL) {
            windows[i]->repaint(windows[i]);
        }
    }
}
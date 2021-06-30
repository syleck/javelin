#include "term.h"
#include "../../io/tty.h"
#include "../state.h"

static uint16_t* buffer;

void term_repaint(window* win) {
    for(int i = 0; i < CONSOLE_WIDTH; i++) {
        for(int j = 0; j < CONSOLE_HEIGHT; j++) {
            uint16_t chara = buffer[i + CONSOLE_WIDTH * j];
            win->toolkit->drawchar(win,i*CHAR_WIDTH,j*CHAR_HEIGHT,chara);
        }
    }
}

window* term_create() {
    window* win = kmalloc(sizeof(window),"KLogTerminal");
    buffer = get_info(SYSINFO_TTY_MEMADDR);
    win->ownerpid = 1;
    win->repaint = term_repaint;
    win->h = CONSOLE_HEIGHT*CHAR_HEIGHT;
    win->w = CONSOLE_WIDTH*CHAR_WIDTH;
    win->ox = win->x = 100;
    win->oy = win->y = 100;
    win->visible = true;
    win->title = "KLogTerminal";
    return win;
}
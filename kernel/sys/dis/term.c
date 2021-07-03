#include "term.h"
#include "../../io/tty.h"
#include "../state.h"
#include "../../module.h"

MODULE("KLOGTERM")
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

static uint16_t* buffer;

void term_repaint(window* win) {
    for(int i = 0; i < CONSOLE_WIDTH; i++) {
        for(int j = 0; j < CONSOLE_HEIGHT; j++) {
            uint16_t chara = buffer[i + CONSOLE_WIDTH * j];
            uint8_t chara_8 = (uint8_t)chara;
            win->toolkit->drawchar(win,i*CHAR_WIDTH,j*CHAR_HEIGHT,chara_8);
        }
    }
}

window* term_create() {
    window* win = kmalloc(sizeof(window),"KLogTerminal");
    buffer = get_info(SYSINFO_TTY_MEMADDR);
    mprintf("Term Buffer at %x\n",buffer);
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
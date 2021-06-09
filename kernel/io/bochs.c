#include "stdio.h"
#include "bochs.h"
#include "tty.h"
#include "../module.h"
#include "../x86/asm.h"
#include "../drv/device.h"

MODULE("BOCHSIO");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

void bochs_putch(char i) {
    outb(0xe9,i);
}

void bochs_putstr(char* i, int l) {
    int j = 0;
    while(l != 0) {
        bochs_putch(*(i+j));
        j++; l--;
    }
}

io_struct bochs = {
    .write_byte = bochs_putch,
    .write_stream = bochs_putstr,
    .cmode = IO_INPUT,
};

void init_bochs() {
    bochs_handle = &bochs;
    add_simple_text("bochs",bochs);
    add_echo_tty(&bochs);
}
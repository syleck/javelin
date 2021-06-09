#include "keyboard.h"
#include "../x86/int/irq.h"
#include "../x86/asm.h"
#include "device.h"
#include "../module.h"
#include "../io/stdio.h"
#include <stdbool.h>

bool keyboard_interrupted = false;

MODULE("KEYBOARD")
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

char get_key() {
    
}

io_struct kbstruct = {
    .read_byte = get_key
};

void kb_int_handler(int irq) {
    if(irq == 1) {
        keyboard_interrupted = true;
    }   
}

void init_keyboard() {
    set_irq(kb_int_handler,1);
    add_simple_text("kbd",kbstruct);
}

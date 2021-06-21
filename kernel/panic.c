#include "module.h"
#include "sys/state.h"

MODULE("PANIC");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

int panic(char* reason) {
    asm("sti");
    set_state(SYSTEM_PANIC);
    mputs("System is in a panicked state\n");
    for(;;) {

    }
}
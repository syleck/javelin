#include "module.h"
#include "sys/state.h"
#include "sys/framebuffer.h"
#include "drv/keyboard.h"
#include <stdbool.h>

MODULE("PANIC");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

bool panicState = false;
int panic(char* reason) {
    set_state(SYSTEM_PANIC);
    mputs("System is in a panicked state\n");
    printf("\nFor users\n");
    printf("Your system is in a borked (panicked) state.\n");
    printf("This is usually because a driver running in kernel mode or general bugs in the kernel caused the system to halt\n");
    printf("Any work that is unsaved or otherwise hasnt written itself to disk has lost its data.\n");
    printf("\nFor devs\n");
    printf("Fix the bloody kernel, damnit! The exceptions, registers and stack trace is above.\n");
    printf("If you're at a loss of where to commit, or want the sourcecode, its at <https://github.com/syleck/javelin> at the moment.\n");
    printf("\n\nStriking key 'r' will enter the monitor.\n");
    asm("sti");
    panicState = true;
    while(panicState) {
        char i = get_key();
        if(i == 'r') {
            monitor();
        }
    }
}

void monitor() {
    printf("SYSTEM MONITOR\n");
    bool state = true;
    while(state) {
        char input[16];
        read_string(input,16);
        if(strcmp(input,"ExitPanic")==0) {
            panicState = false;
            printf("EXPERIMENTAL\n");
        }
    }
}
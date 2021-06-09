#include "task.h"
#include "../module.h"

MODULE("TASK")
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

void* user_entry = 0;
extern void jump_usermode();

void usermode_task_sample() {
    for(int i = 0; i < 512; i++) {
        printf("Test %i\n",i);
    }
}

void run_task(void* code) {
    mputs("Starting task\n");
    user_entry = code;
    mputs("Entering usermode\n");
    jump_usermode();
}
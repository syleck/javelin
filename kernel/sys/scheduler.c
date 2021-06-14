#include "scheduler.h"
#include "../module.h"
#include "../mem.h"
#include "linked_list.h"
#include "../io/tty.h"
#include "../x86/task.h"
#include "../stdlib.h"
#include <stdint.h>
#include <stdbool.h>

MODULE("SCHEDULER");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com")
MODULE_LICENSE("AGPL");

struct task {
    uint8_t permissions;
    char* name;
    uint32_t tasktime;
    uint32_t settime;
    uint16_t ttymem[CONSOLE_WIDTH*CONSOLE_HEIGHT];
    struct regs registers;
};

struct task kernel_task;
struct task* current_task = 0;
struct task* previous_task = 0;
bool scheduler_state = false;
uint64_t sched_calls = 0;

void printsched() {
    int otc = terminal_col;
    int otr = terminal_row;
    update_cursor(0,0);
    tty_setcolor(TERM_SCLOR);
    printf("                                ");
    update_cursor(0,0);
    printf("JAVELIN;Task: %s;",current_task->name);
    tty_setcolor(TERM_COLOR);
    terminal_col = otc;
    terminal_row = otr;
}


void scheduler_yield(struct regs *regs) {
    if(!scheduler_state) {
        scheduler_init(regs);
        return;
    }
    if(sched_calls % 1000 != 0)
        printsched();
    sched_calls++;
    if(current_task->tasktime == 0) {
        //current_task = *((struct task*)tasks->next->data);
        current_task->tasktime = current_task->settime;
        memcpy(regs,&current_task->registers,sizeof(struct regs));
    } else {
        current_task->tasktime--;
    }
}

void scheduler_init(struct regs *kregs) {
    kernel_task.permissions = 0xff;
    kernel_task.registers = *kregs;
    kernel_task.settime = 2;
    kernel_task.tasktime = kernel_task.settime;
    kernel_task.name = "kernel";
    current_task = &kernel_task;
    scheduler_state = true;
}
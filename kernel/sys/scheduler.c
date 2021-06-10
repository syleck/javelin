#include "scheduler.h"
#include "../module.h"
#include "../mem.h"
#include "linked_list.h"
#include "../io/tty.h"
#include "../stdlib.h"
#include <stdint.h>
#include <stdbool.h>

MODULE("SCHEDULER");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com")
MODULE_LICENSE("AGPL");

linkedlist* tasks;
struct task {
    uint8_t permissions;
    uint32_t tasktime;
    uint32_t settime;
    char name[16];
    struct regs registers;
};

struct task* kernel_task;
struct task current_task;
bool scheduler_state = false;

void scheduler_yield(struct regs *regs) {
    if(!scheduler_init)
        return scheduler_init(regs);
    int otc = terminal_col;
    int otr = terminal_row;
    update_cursor(0,0);
    if(kernel_task->tasktime == 0) {
        printf("switching");
        current_task = *((struct task*)tasks->next->data);
        current_task.tasktime = current_task.settime;
        memcpy(regs,&current_task.registers,sizeof(struct regs));
    } else {
        printf("%s (%x) %ims left",current_task.name,current_task,current_task.tasktime);
        current_task.tasktime--;
    }
    terminal_col = otc;
    terminal_row = otr;
}

void scheduler_init(struct regs *kregs) {
    kernel_task = (struct task*)malloc(sizeof(struct task));
    tasks = ll_create(kernel_task);
    kernel_task->permissions = 0xff;
    kernel_task->registers = *kregs;
    kernel_task->settime = 2;
    kernel_task->tasktime = kernel_task->settime;
    tasks->next = tasks;
    memcpy(kernel_task->name,"kernel",8);
    current_task = *kernel_task;
}
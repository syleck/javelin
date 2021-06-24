#include "scheduler.h"
#include "../module.h"
#include "../mem.h"
#include "linked_list.h"
#include "../io/tty.h"
#include "../x86/task.h"
#include "../stdlib.h"
#include "../string.h"
#include "dis/wm.h"
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
    uint16_t ttymem[2048];
    struct regs registers;
};

struct task kernel_task;
struct task* current_task = 0;
struct task* previous_task = 0;
bool scheduler_state = false;
uint64_t sched_calls = 0;

void scheduler_paint(window* wind) {
    wind->toolkit->drawtext(wind,1,1,"Scheduler");
    wind->toolkit->drawtext(wind,1,17,"Task:");
    wind->toolkit->drawtext(wind,41,17,current_task->name);
}

void scheduler_yield(struct regs *regs) {
    if(!scheduler_state) {
        scheduler_init(regs);
        return;
    }
    sched_calls++;
    if(current_task->tasktime == 0) {
        //current_task = *((struct task*)tasks->next->data);
        current_task->tasktime = current_task->settime;
        memcpy(regs,&current_task->registers,sizeof(struct regs));
    } else {
        current_task->tasktime--;
    }
    if(sched_calls % 100 == 0)
	    repaint_all();
}

void scheduler_init(struct regs *kregs) {
    kernel_task.permissions = 0xff;
    kernel_task.registers = *kregs;
    kernel_task.settime = 2;
    kernel_task.tasktime = kernel_task.settime;
    kernel_task.name = "kernel";
    current_task = &kernel_task;
    scheduler_state = true;

    window* taskwin = kmalloc(sizeof(taskwin),"SchedulerWindow");
    taskwin->x = taskwin->ox = 200;
    taskwin->y = taskwin->oy = 50;
    taskwin->w = 300;
    taskwin->h = 500;
    taskwin->ownerpid = 1;
    taskwin->repaint = scheduler_paint;
    taskwin->title = "Scheduler Info";
    taskwin->visible = true;
    new_window(taskwin);
}
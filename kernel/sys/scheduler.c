#include "scheduler.h"
#include "../module.h"
#include "../mem.h"
#include "linked_list.h"
#include "../io/tty.h"
#include "../x86/task.h"
#include "../x86/asm.h"
#include "../stdlib.h"
#include "../string.h"
#include "dis/wm.h"
#include "time.h"
#include "../drv/rtc.h"
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


void scheduler_yield(struct regs *regs) {
    if(!scheduler_state) {
        scheduler_init(regs);
        return;
    }
    updtime();
    sched_calls++;
    if(current_task->tasktime == 0) {
        //current_task = *((struct task*)tasks->next->data);
        current_task->tasktime = current_task->settime;
        memcpy(regs,&current_task->registers,sizeof(struct regs));
    } else {
        current_task->tasktime--;
    }
    drawstring("Scheduler",0,12,0x000000,0x606060);
    char calls[16];
    itoa(sched_calls,16,calls);
    drawstring(calls,0,20,0x000000,0x606060);
    drawstring("Javelin was built at " DATE ", version " VERSION,0,28,0x000000,0x606060);
    drawstring("Task: ",0,36,0x000000,0x606060);
    time tim = gettime();
    char seconds[3];
    char minutes[3];
    char hours[3];
    itoa(tim.seconds,10,seconds);
    itoa(tim.minutes,10,minutes);
    itoa(tim.hours,10,hours);
    int o = 0;
    drawstring("         ",0,48,0x000000,0x606060);
    drawstring(hours,0,48,0x000000,0x606060);
    o += (strlen(hours)+1)*CHAR_WIDTH;
    drawstring(minutes,o,48,0x000000,0x606060);
    o += (strlen(minutes)+1)*CHAR_WIDTH;
    drawstring(seconds,o,48,0x000000,0x606060);
    o += (strlen(seconds)+1)*CHAR_WIDTH;
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
}
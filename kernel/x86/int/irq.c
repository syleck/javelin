#include "../idt.h"
#include "../asm.h"
#include "../../module.h"
#include "irq.h"
#include "../../sys/scheduler.h"

MODULE("IRQ")
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

struct {
	void (*program)(int);
	int irq;
} handler[15];

uint64_t ticks = 0;

void sleep(int ms) {
	asm("sti");
	ms = ms + ticks;
	while(ticks != ms) {
		asm("nop");
	}
}

extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

void irq_remap()
{
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}

void irq_install()
{
	irq_remap();

	idt_set_gate(32, (unsigned)_irq0, 0x08, 0x8E);
	idt_set_gate(33, (unsigned)_irq1, 0x08, 0x8E);
	idt_set_gate(34, (unsigned)_irq2, 0x08, 0x8E);
	idt_set_gate(35, (unsigned)_irq3, 0x08, 0x8E);
	idt_set_gate(36, (unsigned)_irq4, 0x08, 0x8E);
	idt_set_gate(37, (unsigned)_irq5, 0x08, 0x8E);
	idt_set_gate(38, (unsigned)_irq6, 0x08, 0x8E);
	idt_set_gate(39, (unsigned)_irq7, 0x08, 0x8E);
	idt_set_gate(40, (unsigned)_irq8, 0x08, 0x8E);
	idt_set_gate(41, (unsigned)_irq9, 0x08, 0x8E);
	idt_set_gate(42, (unsigned)_irq10, 0x08, 0x8E);
	idt_set_gate(43, (unsigned)_irq11, 0x08, 0x8E);
	idt_set_gate(44, (unsigned)_irq12, 0x08, 0x8E);
	idt_set_gate(45, (unsigned)_irq13, 0x08, 0x8E);
	idt_set_gate(46, (unsigned)_irq14, 0x08, 0x8E);
	idt_set_gate(47, (unsigned)_irq15, 0x08, 0x8E);
}

void set_irq(void (*program)(int), int id) {
	mprintf("Set interrupt %i to %x\n",id,program);
	handler[id].irq = id;
	handler[id].program = program;
}

int current_irq = 0;
int get_irqc() {
	return current_irq;
}

void _irq_handler(struct regs r)
{
	current_irq = r.int_no;
    switch(r.int_no) {
		case 0x20:
			scheduler_yield(&r);
			ticks++;
			break;
        default:   
			if(handler[r.int_no-0x1f].program != 0) {
				handler[r.int_no-0x1f].program(r.int_no);
			}
			mprintf("IRQ: %x\n",r.int_no);
			OOPS("Unknown IRQ");
            break;
    }
	if (r.int_no >= 40)
		outb(0xA0, 0x20);
	outb(0x20,0x20);
	current_irq = 0;
}
#ifndef IRQ_H
#define IRQ_H
#include "isr.h"
#include <stdbool.h>

void irq_install();
void sleep(int ms);
void set_irq(void (*program)(int), int id);
int get_irqc();

#endif
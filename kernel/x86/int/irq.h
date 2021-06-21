#ifndef IRQ_H
#define IRQ_H
#include "isr.h"
#include <stdbool.h>
#include <stdint.h>

void irq_install();
void sleep(float ms);
uint64_t get_ticks();
void set_irq(void (*program)(int), int id);
int get_irqc();

#endif
#ifndef IDT_H
#define IDT_H

void _idt_load();
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void idt_install();

#endif
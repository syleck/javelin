#include "../module.h"

MODULE("IDT")
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

struct idt_entry
{
	unsigned short base_lo;
	unsigned short sel;	
	unsigned char always0;	
	unsigned char flags;	
	unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr
{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr _idtp;

extern void _idt_load();

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
	/* Assign the base values */
	idt[num].base_lo = (base & 0xFFFF);
	idt[num].base_hi = (base >> 16) & 0xFFFF;

	/* Set the other fields */
	idt[num].sel = sel;
	idt[num].always0 = 0;
	idt[num].flags = flags;
}

/* Installs the IDT */
void idt_install()
{
	_idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
	_idtp.base = &idt;

	memset((void*)&idt, 0, sizeof(struct idt_entry) * 256);

	_idt_load();
}
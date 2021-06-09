#include <stdint.h>
#include "../io/stdio.h"
#include "../module.h"

MODULE("GDT");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

struct {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed)) gdt_entries[6];

struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) _gdt_ptr;

struct tss_entry_struct {
	uint32_t prev_tss; // The previous TSS - with hardware task switching these form a kind of backward linked list.
	uint32_t esp0;     // The stack pointer to load when changing to kernel mode.
	uint32_t ss0;      // The stack segment to load when changing to kernel mode.
	// Everything below here is unused.
	uint32_t esp1; // esp and ss 1 and 2 would be used when switching to rings 1 or 2.
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt;
	uint16_t trap;
	uint16_t iomap_base;
} __attribute__((packed));

struct tss_entry_struct tss_entry;

extern void _gdt_flush();
extern void _tss_flush();
extern uint32_t _get_ksr();

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
	mprintf("Inserting gate %i, base: %x, limit: %x, access: %x, granularity: %x\n", num, base, limit, access, granularity);
	gdt_entries[num].base_low = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high = (base >> 24) & 0xFF;
	gdt_entries[num].limit_low = (limit & 0xFFFF);
	gdt_entries[num].granularity |= (granularity & 0xF0);
	gdt_entries[num].access = access;
}

void tss_install(int num, uint32_t ss0, uint32_t esp0) {
	uint32_t base = (uint32_t)&tss_entry;
	uint32_t limit = sizeof(tss_entry);
	gdt_set_gate(num,base,limit,0xE9,0x00);
	tss_entry.ss0 = 0x10;
	tss_entry.esp0 = 0;
}

void tss_set_kernel_stack(unsigned int stack)
{
	tss_entry.esp0 = stack;
}

void gdt_install() {
	_gdt_ptr.limit = sizeof(gdt_entries)-1;
	_gdt_ptr.base = (uint32_t)&gdt_entries;
	gdt_set_gate(0,0,0,0,0); // gdt entry 1 has to be null
	gdt_set_gate(1,0,0xFFFFFFFF,0x9A,0xCF); // CS
	gdt_set_gate(2,0,0xFFFFFFFF,0x92,0xCF); // DS
	gdt_set_gate(3,0,0xFFFFFFFF,0xFA,0xCF); // task CS
	gdt_set_gate(4,0,0xFFFFFFFF,0xF2,0xCF); // task DS
	tss_install(5,0x8,0x10);
	_gdt_flush();
	_tss_flush();
}

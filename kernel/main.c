#include <cpuid.h>
#include "x86/gdt.h"
#include "x86/task.h"
#include "x86/int/isr.h"
#include "x86/int/irq.h"
#include "x86/bits16/bios.h"
#include "x86/boot/grub.h"
#include "x86/idt.h"
#include "x86/asm.h"
#include "fs/fs.h"
#include "pci/pci.h"
#include "io/tty.h"
#include "io/serial.h"
#include "io/bochs.h"
#include "io/stdio.h"
#include "sys/random.h"
#include "sys/snd.h"
#include "sys/framebuffer.h"
#include "drv/device.h"
#include "drv/keyboard.h"
#include "drv/adlib.h"
#include "drv/sb16.h"
#include "fs/iso9660.h"
#include "injen/injen.h"
#include "module.h"

MODULE("KERNEL");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

int kernel_main(uint32_t bleax, uint32_t blebx) {
	idt_install();
	isr_install();
	irq_install();

	init_tty();
	fb_init();

	multiboot_init(bleax,blebx);
	fb_clear();

	init_serial();
	init_bochs();
	printf("==javelin==\nbuilt at: %s\n",__DATE__);
	printf("Special flags: ");
	#ifdef PANIC_ON_OOPS
	printf("PANIC_ON_OOPS ");
	#endif
	#ifdef ONT_DO_MODULES
	printf("DONT_DO_MODULES (ONT_DO_MODULES) ");
	#endif
	#ifdef VERBOSE
	printf("VERBOSE ");
	#endif
	#ifdef NO_SCHED
	printf("NO_SCHED (PIT is off!) ");
	#endif
	printf("\n");

	struct {
		uint32_t ebx;
		uint32_t edx;
		uint32_t ecx;
		char terminat; 
		uint32_t eax;
	} cpuidregs;
	cpuidregs.terminat = 0;
	__cpuid(0,cpuidregs.eax,cpuidregs.ebx,cpuidregs.ecx,cpuidregs.edx);
	printf("CPUID: %s\n",&cpuidregs);

	gdt_install();

	#ifndef NO_SCHED
	// do pit init here cause im lazy to make another file
	uint16_t divisor = (uint16_t) (1193180 / 10000); // 10000Hz will make the PIT fire IRQ #0 every .1ms
    uint8_t low = (uint8_t) (divisor & 0xFF);
    uint8_t high = (uint8_t) ((divisor >> 8) & 0xFF);
	outb(0x43, 0x36);
    outb(0x40, low);
    outb(0x40, high);
	#endif

	init_keyboard();
	pci_install();
	//init_ata_pio();
	init_random();
	//init_sound();

	asm("sti");
	for(;;) {
		sleep(1);
	}
	return 0;
}

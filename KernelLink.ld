ENTRY(_start)
SECTIONS
{
	. = 1M;

	_kernel_start = .;
	.multiboot BLOCK (4K) : ALIGN (4K) {		
		*(.multiboot)
		. = ALIGN(8);
		KEEP(*(.multiboot))
		*(.multiboot_header)
		. = ALIGN(8);
		KEEP(*(.multiboot_header))
		*(.multiboot.text)
		. = ALIGN(8);
		KEEP(*(.multiboot.text))
	}
	. += 0xC0000000;
	.text ALIGN (4K) : AT(ADDR(.text)-0xC0000000)
	{
		*(.text)
	}

	.rodata BLOCK(4K) : AT (ADDR (.rodata) - 0xC0000000) ALIGN (4K)
	{
		*(.rodata)
	}

	.data BLOCK(4K) : AT (ADDR (.data) - 0xC0000000) ALIGN (4K)
	{
		*(.data)		
	}

	.bss BLOCK(4K) : AT (ADDR (.bss) - 0xC0000000) ALIGN (4K)
	{
		*(.COMMON)
		*(.bss)
	}
	_kernel_end = .;
}

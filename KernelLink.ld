ENTRY(_start)
SECTIONS
{
	. = 1M;
	_kernel_start = .;

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		. = ALIGN(8);
		KEEP(*(.multiboot))
		*(.multiboot_header)
		. = ALIGN(8);
		KEEP(*(.multiboot_header))
		*(.multiboot.text)
		. = ALIGN(8);
		KEEP(*(.multiboot.text))
		_DATA_START_ADDR = .;
		*(.text)
	}

	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		_DATA_START_ADDR = .;
		*(.data)		
		_DATA_END_ADDR = .;
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(.COMMON)
		*(.bss)
		_BSS_END_ADDR = .;
	}
	_kernel_end = .;
}

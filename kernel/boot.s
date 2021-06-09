.set MAGIC, 0xE85250D6
.set ARCH, 0x0
.set STACK_SIZE, 2 * 1000 * 1000
.set EBP_FIELD, 0xB00B1E5

.section .multiboot
.global _multiboot_header
.align 4
_multiboot_header:
	.long MAGIC
	.long ARCH
	.long _multiboot_header - _multiboot_end
	.long -(MAGIC + ARCH + (_multiboot_header - _multiboot_end))
_multiboot_end:
_terminating_multiboot_tag:
	.word 0
	.word 0
	.long 8

.section .bss
.align 16
stack_bottom:
	.skip STACK_SIZE
stack_top:

.section .text
.global _start
.type _start,@function
_start:
	mov $stack_top,%esp
	xor %ebp, %ebp
	mov $EBP_FIELD,%ebp
	call kernel_main
	cli
1:	hlt
	jmp 1b

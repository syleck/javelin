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
        .long (_multiboot_header - _multiboot_end)
        .long -(MAGIC + ARCH + (_multiboot_header - _multiboot_end))

_multiboot_information:
        .word 1
        .word 0
        .long _multiboot_information_end - _multiboot_information
        .long 1
_multiboot_information_end:

        .align 8
_multiboot_framebuffer:
		.word 5
		.word 0
		.long _multiboot_framebuffer_end - _multiboot_framebuffer 
		.long 0
		.long 0
		.long 32
_multiboot_framebuffer_end:

        .align 8
_terminating_multiboot_tag:
        .word 0
        .word 0
        .long _terminating_multiboot_tag_end-_terminating_multiboot_tag
_terminating_multiboot_tag_end:

_multiboot_end:

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
	push %ebx
	push %eax
	call kernel_main
	cli
1:	hlt
	jmp 1b
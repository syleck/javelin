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

.section .bss, "aw", @nobits
.align 16
stack_bottom:
	.skip STACK_SIZE
stack_top:
.align 4096
boot_page_directory:
	.skip 4096
boot_page_table1:
	.skip 4096

.global _start
.type _start,@function
.section .multiboot.text
_start:
	movl $0, %esi
	movl $1023, %ecx
1:  cmpl $_kernel_start, %esi
	jl 2f
	cmpl $(_kernel_end - 0xC0000000), %esi
	jge 3f
	movl %esi, %edx
	orl $0x003, %edx
	movl %edx, (%edi)
2:	addl $4096, %esi
	addl $4, %edi
3:	jmp 1b
	movl $(0xFD000000 | 0x003), boot_page_table1 - 0xC0000000 + 1023 * 4
	movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
	movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4
	movl $(boot_page_directory - 0xC0000000), %ecx
	movl %ecx, %cr3
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0
	lea 4f, %ecx
	jmp *%ecx
.section .text
4:	movl $0, boot_page_directory + 0
	movl %cr3, %ecx
	movl %ecx, %cr3
	mov $stack_top,%esp
	xor %ebp, %ebp
	mov $EBP_FIELD,%ebp
	push %ebx
	push %eax
	call kernel_main
	cli
1:	hlt
	jmp 1b

.text
.globl loadPageDirectory
loadPageDirectory:
	push %ebp
	mov %esp, %ebp
	mov 8(%esp), %eax
	mov %eax, %cr3
	mov %ebp, %esp
	pop %ebp
	ret

.text
.globl enablePaging
enablePaging:
	push %ebp
	mov %esp, %ebp
	mov %cr0, %eax
	or $0x80000000, %eax
	mov %eax, %cr0
	mov %ebp, %esp
	pop %ebp
	ret
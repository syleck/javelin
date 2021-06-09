[global _gdt_flush]
extern _gdt_ptr
_gdt_flush:
	xchg bx, bx
	lgdt [_gdt_ptr]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:_gdt_flushend
_gdt_flushend:
	ret

[global _tss_flush]
_tss_flush:
	xchg bx, bx
	mov ax, 0x2B
	ltr ax
	ret

[global jump_usermode]
extern user_entry
jump_usermode:
	mov ax, (4 * 8) | 3 ; user data segment with RPL 3
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax ; sysexit sets SS
 
	; setup wrmsr inputs
	xor edx, edx ; not necessary; set to 0
	mov eax, 0x100008 ; SS=0x10+0x10=0x20, CS=0x8+0x10=0x18
	mov ecx, 0x174 ; MSR specifier: IA32_SYSENTER_CS
	wrmsr ; set sysexit segments
 
	; setup sysexit inputs
	mov edx, user_entry ; to be loaded into EIP
	mov ecx, esp ; to be loaded into ESP
	sysexit

_usermode_test:
	mov word [0xb8001],0
	jmp $
	ret

[global _get_ksr]
_get_ksr:
	mov eax, ebp
	ret

[global _idt_load]
extern _idtp
_idt_load:
	lidt [_idtp]
	ret
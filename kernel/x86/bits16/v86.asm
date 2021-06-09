[global _BITS16_START_ADDR]
_BITS16_START_ADDR:
; put all data between these
[bits 16]
[global hires_textmode]
hires_textmode:
    lidt [0]
    mov ah,0x11
    int 0x10

[global _BITS16_END_ADDR]
_BITS16_END_ADDR:

[bits 32]
[global enter_v86]
enter_v86:
    mov ebp, esp
    push dword  [ebp+4]        ; ss
    push dword  [ebp+8]        ; esp
    pushfd                     ; eflags
    or dword [esp], (1 << 17)  ; set VM flags
    push dword [ebp+12]        ; cs
    push dword  [ebp+16]       ; eip
    iret
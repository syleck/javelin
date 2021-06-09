#ifndef ISR_H
#define ISR_H

struct regs
{
	unsigned int ds;					
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;	
	unsigned int int_no, err_code;		
	unsigned int eip, cs, eflags, useresp, ss;           
};

void isr_install();

#endif
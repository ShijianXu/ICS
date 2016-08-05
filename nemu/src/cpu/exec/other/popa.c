#include "cpu/exec/helper.h"

make_helper(popa) {
	cpu.edi=swaddr_read(cpu.esp,4,2);
	cpu.esp+=4;

	cpu.esi=swaddr_read(cpu.esp,4,2);
	cpu.esp+=4;

	cpu.ebp=swaddr_read(cpu.esp,4,2);
	cpu.esp+=4;

	swaddr_read(cpu.esp,4,2);
	cpu.esp+=4;

	cpu.ebx=swaddr_read(cpu.esp,4,2);
	cpu.esp+=4;

	cpu.edx=swaddr_read(cpu.esp,4,2);
	cpu.esp+=4;

	cpu.ecx=swaddr_read(cpu.esp,4,2);
	cpu.esp+=4;

	cpu.eax=swaddr_read(cpu.esp,4,2);
	cpu.esp+=4;

	print_asm("popa");
	return 1;
}

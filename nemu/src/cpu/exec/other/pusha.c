#include "cpu/exec/helper.h"

make_helper(pusha) {
	uint32_t temp = cpu.esp;

	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eax,2);

	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.ecx,2);

	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.edx,2);

	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.ebx,2);

	cpu.esp-=4;
	swaddr_write(cpu.esp,4,temp,2);

	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.ebp,2);

	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.esi,2);

	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.edi,2);

	print_asm("pusha");
	return 1;
}

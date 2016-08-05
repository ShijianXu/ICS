#include "cpu/exec/helper.h"

make_helper(iret) {
	cpu.eip=swaddr_read(cpu.esp,4,2);
	cpu.esp+=4;

	cpu.sreg[1].Sreg=swaddr_read(cpu.esp,2,2);
	cpu.esp+=4;

	cpu.EFLAGS=swaddr_read(cpu.esp,4,2);
	cpu.esp+=4;

	print_asm("iret");
	return 0;
}

#include "cpu/exec/helper.h"


make_helper(ret) {
	
	cpu.eip=swaddr_read(cpu.esp,4,2);

	cpu.esp+=4;

	print_asm("ret");

	return 1;
}


make_helper(ret_w) {
	cpu.eip=swaddr_read(cpu.esp,4,2);
	cpu.esp+=4;
	
	cpu.esp+=op_src->val;
print_asm("ret %x",op_src->val);
	return 1;
}

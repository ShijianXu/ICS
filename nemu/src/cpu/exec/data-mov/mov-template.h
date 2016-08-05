#include "cpu/exec/template-start.h"

#define instr mov

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

/*
make_helper(mov_cr02r) {
	uint8_t index = instr_fetch(eip+1,1);
	REG(index) = cpu.cr0;
	
//	char cr0_name[10]="cr0";
//	print_asm("mov  %%%s,%%%s",cr0_name,REG_NAME(index);
	return 1;
}

make_helper(mov_r2cr0) {
	uint8_t index = instr_fetch(eip+1,1);
	cpu.cr0 = REG(index);
	
//	char cr0_name[10]="cr0";
//	print_asm("mov  %%%s,%%%s",REG_NAME(index),cr0_name);
	return 1;
}
*/
#include "cpu/exec/template-end.h"

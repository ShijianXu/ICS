#include "cpu/exec/template-start.h"
#include "nemu.h"
#define instr movcr2r

static void do_execute() {
	if(op_src->reg == 0)
		REG(op_dest->reg) = cpu.cr0;
	else if(op_src->reg == 3)
		REG(op_dest->reg) = cpu.cr3;
	else
		assert(0);
	print_asm_template2();
}

#if DATA_BYTE == 4
make_instr_helper(r2rm);
#endif
#include "cpu/exec/template-end.h"

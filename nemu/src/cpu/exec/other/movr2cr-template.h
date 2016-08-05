#include "cpu/exec/template-start.h"
#include "nemu.h"
#define instr movr2cr

void init_tlb();

static void do_execute() {
	if(op_dest->reg == 0)
		cpu.cr0 = REG(op_src->reg);
	else if(op_dest->reg == 3)
	{
		cpu.cr3 = REG(op_src->reg);
		init_tlb();	
	}
	print_asm_template2();
}

#if DATA_BYTE == 4
make_instr_helper(rm2r);
#endif
#include "cpu/exec/template-end.h"

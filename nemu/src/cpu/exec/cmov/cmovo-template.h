#include "cpu/exec/template-start.h"

#define instr cmovo

static void do_execute() {
	DATA_TYPE temp=op_src->val;
	if(cpu.OF==1)
		OPERAND_W(op_dest,temp);

	print_asm_template2();
}

make_instr_helper(rm2r);

#include "cpu/exec/template-end.h"

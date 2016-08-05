#include "cpu/exec/template-start.h"

#define instr movsw

static void do_execute() {
	
	int temp=op_src->val;
	temp=(temp<<16)>>16;	
	OPERAND_W(op_dest,temp);
	print_asm_template2();
}

make_instr_helper(rm12r)

#undef instr


#define instr movsb

static void do_execute() {
	int temp=op_src->val;
	temp=(temp<<24)>>24;
	OPERAND_W(op_dest,temp);
	print_asm_template2();
}

make_instr_helper(rm12r)

#undef instr
#include "cpu/exec/template-end.h"

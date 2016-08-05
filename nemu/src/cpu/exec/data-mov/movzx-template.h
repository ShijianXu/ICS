#include "cpu/exec/template-start.h"

#define instr movzb

static void do_execute() {
	
	unsigned temp=op_src->val & 0xff;
	OPERAND_W(op_dest,temp);
print_asm_template2();
}

make_instr_helper(rm12r)

#undef instr

#define instr movzw
static void do_execute() {
	unsigned temp=op_src->val & 0xffff;
	OPERAND_W(op_dest,temp);
	print_asm_template2();
}

make_instr_helper(rm12r)

#define instr movzw


#undef instr
#include "cpu/exec/template-end.h"

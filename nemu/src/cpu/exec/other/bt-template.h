#include "cpu/exec/template-start.h"
#include "nemu.h"
#define instr bt

static void do_execute() {
//Log("op_src %x	op_src2 %x op_dest %x",op_src->val, op_src2->val, op_dest->val);
	uint32_t temp = (op_dest->val >> op_src->val) & 0x1;
//Log("temp %x",temp);
	cpu.CF = temp; 
//Log("CF %x",cpu.CF);
	print_asm_template2();

}

make_instr_helper(r2rm);

#include "cpu/exec/template-end.h"

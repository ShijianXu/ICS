#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {

	if(op_src->type==OP_TYPE_IMM)
	{
		cpu.eip=cpu.eip+(DATA_TYPE_S)op_src->val;
		if(DATA_BYTE==2)
			cpu.eip=cpu.eip & 0x0000ffff;	
	}
	if(DATA_BYTE==2 || DATA_BYTE==4)
		if(op_src->type==OP_TYPE_MEM || op_src->type==OP_TYPE_REG)
		{
			DATA_TYPE result;
			result=(op_src->val);
			if(DATA_BYTE==2)
					result&=0x0000ffff;
		
		if(op_src->type==OP_TYPE_REG)
			cpu.eip=result-2;
		else if(op_src->type==OP_TYPE_MEM)
			cpu.eip=result-7;
		}
	print_asm_template1();
}

make_instr_helper(i);
#if DATA_BYTE==2 || DATA_BYTE==4
make_instr_helper(rm);
#endif
#include "cpu/exec/template-end.h"

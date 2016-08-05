#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	uint32_t result;
	if(DATA_BYTE==2)
	{
		result=swaddr_read(cpu.esp,2,2);
		OPERAND_W(op_src, result);
		cpu.esp+=2;
	}
	else if(DATA_BYTE==4)
	{
		result=swaddr_read(cpu.esp,4,2);
		OPERAND_W(op_src, result);
//	printf("%x\n",op_dest->val);
		cpu.esp=cpu.esp+4;
	}
	print_asm_template1();
}
#if DATA_BYTE==2 || DATA_BYTE==4
make_instr_helper(rm)
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr jle

static void do_execute() {
	if(cpu.ZF==1 || cpu.SF!=cpu.OF)
	{
		cpu.eip=cpu.eip+(DATA_TYPE_S)op_src->val;
		if(DATA_BYTE==2)
			cpu.eip=cpu.eip & 0x0000ffff;
	}
	print_asm("jle %x",cpu.eip+2);
}

//#if DATA_BYTE==2
make_instr_helper(i);
//#endif

//#if DATA_BYTE==1 || DATA_BYTE==4
//make_instr_helper(si);
//#endif

#include "cpu/exec/template-end.h"

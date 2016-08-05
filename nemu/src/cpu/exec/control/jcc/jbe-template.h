#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute() {
	if(cpu.ZF==1 || cpu.CF==1)
	{
		cpu.eip=cpu.eip+(DATA_TYPE_S)op_src->val;
		if(DATA_BYTE == 2)
			cpu.eip=cpu.eip & 0x0000ffff;
	}
	print_asm("jbe %x",cpu.eip+2);
}

make_instr_helper(i);

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr ja

static void do_execute() {
	if(cpu.CF==0 && cpu.ZF==0)
	{
		cpu.eip=cpu.eip+(DATA_TYPE_S)op_src->val;
		if(DATA_BYTE==2)
			cpu.eip=cpu.eip & 0x0000ffff;
	}
	print_asm("ja %x",cpu.eip+2);
}

make_instr_helper(i);

#include "cpu/exec/template-end.h"

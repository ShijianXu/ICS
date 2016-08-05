#include "cpu/exec/template-start.h"

#define instr jge

static void do_execute() {
	if(cpu.SF==cpu.OF)
	{
		cpu.eip=cpu.eip+(DATA_TYPE_S)op_src->val;
		if(DATA_BYTE==2)
			cpu.eip=cpu.eip & 0x0000ffff;
	}
	print_asm("jge %x",cpu.eip+2);
}

make_instr_helper(i);

#include"cpu/exec/template-end.h"
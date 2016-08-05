#include "cpu/exec/template-start.h"
#include "nemu.h"
#define instr lgdt

static void do_execute() {
//	swaddr_t addr = instr_fetch(cpu.eip+2,4);
	cpu.GDTR.limit=swaddr_read(op_src->addr,2,3);
	cpu.GDTR.gdt_addr = swaddr_read(op_src->addr+2,4,3);
	
//	cpu.eip-=2;

	print_asm_template1();
}

//#if DATA_BYTE == 4
make_instr_helper(rm);
//#endif

#include "cpu/exec/template-end.h"

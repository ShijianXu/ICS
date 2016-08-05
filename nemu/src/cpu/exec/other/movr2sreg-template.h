#include "cpu/exec/template-start.h"
#include "nemu.h"

#define instr movr2sreg

static void do_execute() {
	uint16_t index=0;
	uint32_t mem_addr=0;
	uint64_t desc=0;

	cpu.sreg[op_dest->reg].Sreg = REG(op_src->reg);
	index=(cpu.sreg[op_dest->reg].Sreg & 0xfff8)>>3;
	mem_addr=cpu.GDTR.gdt_addr+index*8;
	desc=lnaddr_read(mem_addr,4);
	desc=((uint64_t)lnaddr_read(mem_addr+4,4)<<32)+desc;
	cpu.sreg[op_dest->reg].Sreg_cache_val=desc;
	print_asm_template2();
}

#if DATA_BYTE == 4
make_instr_helper(rm2r);
#endif

#include "cpu/exec/template-end.h"

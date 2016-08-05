#include "cpu/exec/helper.h"
#include "nemu.h"
/*
make_helper(lgdtl) {
	swaddr_t addr = instr_fetch(eip+2,4);
	cpu.GDTR.limit = swaddr_read(addr,2,3);
	cpu.GDTR.gdt_addr = swaddr_read(addr+2,4,3);

	print_asm("lgdtl");
	return 6;
}*/

#define DATA_BYTE 2
#include "lgdt-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "lgdt-template.h"
#undef DATA_BYTE

make_helper_v(lgdt_rm)

#include "cpu/exec/template-start.h"
#include "nemu.h"
extern void pio_write(ioaddr_t addr, size_t len, uint32_t data);

make_helper(concat(out_a2i_, SUFFIX)) {
	uint8_t val = instr_fetch(cpu.eip+1, 1);
	pio_write(val, DATA_BYTE, cpu.eax);
	
	print_asm("out");
	return 2;
}

make_helper(concat(out_a2d_, SUFFIX)) {
	pio_write(reg_w(R_DX), DATA_BYTE, cpu.eax);

	print_asm("out");
	return 1;
}

#include "cpu/exec/template-end.h"

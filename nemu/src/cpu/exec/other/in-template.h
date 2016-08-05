#include "cpu/exec/template-start.h"
#include "nemu.h"
extern uint32_t pio_read(ioaddr_t addr, size_t len);

make_helper(concat(in_i2a_, SUFFIX)) {
	uint8_t val = instr_fetch(cpu.eip+1, 1);
	cpu.eax = pio_read(val, DATA_BYTE);

	print_asm("in");
	return 2;
}

make_helper(concat(in_d2a_, SUFFIX)) {
	cpu.eax = pio_read(reg_w(R_DX), DATA_BYTE);

	print_asm("in");
	return 1;
}
#include "cpu/exec/template-end.h"

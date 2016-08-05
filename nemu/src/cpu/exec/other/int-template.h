#include "cpu/exec/template-start.h"
extern void raise_intr(uint8_t);

#define instr intr

static void do_execute() {

	cpu.eip+=2;
	raise_intr(op_src->val);

	print_asm_template1();
}

make_instr_helper(si)

#include "cpu/exec/template-end.h"

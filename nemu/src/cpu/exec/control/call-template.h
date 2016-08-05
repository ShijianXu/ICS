#include "cpu/exec/template-start.h"

#define instr calli

static void do_execute() {
#if DATA_BYTE ==2
	cpu.esp-=2;
	swaddr_write(cpu.esp,2,cpu.eip+DATA_BYTE,2);
	cpu.eip=(cpu.eip+op_src->val) & 0x0000ffff;
#endif
#if DATA_BYTE==4
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip+DATA_BYTE,2);
	cpu.eip=cpu.eip+op_src->val;
#endif
	print_asm_template1();
}

make_instr_helper(i)

#undef instr

#define instr callrm

static void do_execute() {
/*	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip+DATA_BYTE,2);
	cpu.eip=op_src->val-2;*/

	swaddr_t addr = op_src->val;
	Operand temp_reg, temp_rm;
	temp_reg.size = DATA_BYTE;
	temp_rm.size = DATA_BYTE;

	extern int read_ModR_M(swaddr_t, Operand*, Operand*);
	int ins_len = read_ModR_M(cpu.eip+1, &temp_rm, &temp_reg);
	cpu.esp -= DATA_BYTE;

	if(DATA_BYTE == 4)
	{
		swaddr_write(cpu.esp, DATA_BYTE, cpu.eip+ins_len,2);
		cpu.eip = addr - 1 - ins_len;
	}
	else
	{
		swaddr_write(cpu.esp, DATA_BYTE, (cpu.eip+ins_len)&0xffff, 2);
		cpu.eip = (addr & 0xffff) - 1 - ins_len;
	}
	print_asm_template1();
}

make_instr_helper(rm)
#undef instr

#include "cpu/exec/template-end.h"

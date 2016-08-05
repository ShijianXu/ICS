#include "cpu/exec/template-start.h"

#define instr or

static void do_execute () {
	DATA_TYPE result = op_dest->val | op_src->val;
	OPERAND_W(op_dest, result);

	/* TODO: Update EFLAGS. */
//	panic("please implement me");
	cpu.CF=0;
	cpu.OF=0;

	uint32_t r1=result&0xff;
	int i=0;
	int count=0;
	for(;i<8;i++)
	{
		if(r1&0x1)
			count++;
		r1=r1>>1;
	}
	if(count%2==0)
		cpu.PF=1;
	else
		cpu.PF=0;
	
	if(result==0)
		cpu.ZF=1;
	else
		cpu.ZF=0;
	
//	uint32_t r2=result>>31;
//	if(r2==0)
//		cpu.SF=0;
//	else
//		cpu.SF=1;
cpu.SF=MSB(result);
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

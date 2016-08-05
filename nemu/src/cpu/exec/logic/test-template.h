#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
	DATA_TYPE result = op_src->val & op_dest->val;
//	OPERAND_W(op_dest,result);

	cpu.CF=0;
	cpu.OF=0;

	uint32_t r1=result&0xff;
	uint32_t count=0;

	int i=0;
	for(;i<8;i++)
	{
		if(r1&0x1)
			count++;
		r1=	r1>>1;
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
			
//	OPERAND_W(op_dest,result);
	
	print_asm_template2();
}

make_instr_helper(r2rm)
make_instr_helper(i2a)
make_instr_helper(i2rm)
#include "cpu/exec/template-end.h"

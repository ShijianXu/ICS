#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
//	panic("please implement me");
	cpu.CF=op_src->val<1?1:0;
	if(MSB(op_src->val)!=0 && (MSB(result)==0))
		cpu.OF=1;
	else
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
	cpu.SF=MSB(result);

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"

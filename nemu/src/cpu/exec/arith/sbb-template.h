#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute() {	
	int32_t result;
	if(DATA_BYTE==4)
		result = op_dest->val - (op_src->val+cpu.CF);
	else
		result=op_dest->val-((DATA_TYPE_S)op_src->val+cpu.CF);
//	uint32_t s1=(op_dest->val>>31)&0x1;
//	uint32_t s2=(~(op_src->val+cpu.CF)+1)>>31 & 0x1;
//	uint32_t s3=(result>>31)&0x1;
//	uint32_t c=0;

//	if(s1==1 && s2==1)
//		c=1;
//	if((s1==1 && s2==0) && s3==0)
//		c=1;
//	if((s1==0 && s2==1) && s3==0)
//		c=1;

	cpu.CF=op_dest->val<(op_src->val+cpu.CF)?1:0;

//	if((s1==s2) && s1!=s3)
//		cpu.OF=1;
//	else
//		cpu.OF=0;
if(MSB(op_dest->val)!=MSB(op_src->val+cpu.CF) && (MSB(result)==MSB(op_src->val+cpu.CF)))
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

//	uint32_t r2=result>>31;
//	if(r2==0)
//		cpu.SF=0;
//	else
//		cpu.SF=1;

cpu.SF=MSB(result);
	OPERAND_W(op_dest, result);

	print_asm_template2();
}

make_instr_helper(r2rm);
make_instr_helper(rm2r);
make_instr_helper(i2rm);
make_instr_helper(i2a);
#if DATA_BYTE==2 || DATA_BYTE==4
make_instr_helper(si2rm);
#endif

#include "cpu/exec/template-end.h"

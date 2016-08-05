#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {

//	int temp=op_src->val;
	
	int src=(DATA_TYPE_S)op_src->val;
	DATA_TYPE result=op_dest->val - src;


	
	cpu.CF=op_dest->val<src?1:0;

//	if((s1==s2) && s1!=s3)
//		cpu.OF=1;
//	else
//		cpu.OF=0;
if(MSB(op_dest->val)!=MSB(src) && (MSB(result)==MSB(src)))	
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

//cpu.PF=(x&0x1)^0x1;
//cpu.ZF=!result;
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
#if DATA_BYTE==2 || DATA_BYTE==4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

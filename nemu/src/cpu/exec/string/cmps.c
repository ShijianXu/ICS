#include "cpu/exec/helper.h"
#include "cmps.h"
//#include "cpu/exec/template-start.h"

make_helper(cmps_b) {
	int incdec;
	uint32_t dest=swaddr_read(cpu.edi,1,0);
	uint32_t src=swaddr_read(cpu.esi,1,3);

	uint32_t result=src-dest;

	if(cpu.DF==0)
		incdec=1;
	else
		incdec=-1;

	cpu.esi+=incdec;
	cpu.edi+=incdec;


	cpu.CF=src<dest?1:0;
/*	if(MSB(cpu.esi)!=MSB(cpu.edi) && (MSB(result)==MSB(cpu.edi)))
			cpu.OF=1;
	else
			cpu.OF=0;
*/

	uint32_t dest_m=(dest>>31)&0x1;
	uint32_t src_m=(src>>31)&0x1;
	uint32_t result_m=(result>>31)&0x1;
	if((dest_m!=src_m) && (result_m==dest_m))
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

	cpu.SF=(result>>31)&0x1;

	print_asm("cmpsb");
	return 1;
}


make_helper(cmps_w) {
	int incdec;
	uint32_t dest=swaddr_read(cpu.edi,2,0);
	uint32_t src=swaddr_read(cpu.esi,2,3);
	uint32_t result=src-dest;
	
	if(cpu.DF==0)
		incdec=2;
	else
		incdec=-2;
	
	cpu.esi+=incdec;
	cpu.edi+=incdec;


	cpu.CF=src<dest?1:0;
/*	if(MSB(cpu.esi)!=MSB(cpu.edi) && (MSB(result)==MSB(cpu.edi)))
	cpu.OF=1;
	else
		cpu.OF=0;
*/
	uint32_t dest_m=(dest>>31)&0x1;
	uint32_t src_m=(src>>31)&0x1;
	uint32_t result_m=(result>>31)&0x1;
	if((dest_m!=src_m)&&(result_m==dest_m))
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
	
	cpu.SF=(result>>31)&0x1;
	print_asm("cmpsw");
	return 1;
}


make_helper(cmps_l) {
	int incdec;
	uint32_t dest=swaddr_read(cpu.edi,4,0);
	uint32_t src=swaddr_read(cpu.esi,4,3);
	uint32_t result=src-dest;
	
	if(cpu.DF==0)
		incdec=4;
	else
		incdec=-4;
	cpu.CF=src<dest?1:0;		
/*	if(MSB(cpu.esi)!=MSB(cpu.edi) && (MSB(result)==MSB(cpu.edi)))
			cpu.OF=1;
	else
			cpu.OF=0;
*/	
	uint32_t dest_m=(dest>>31)&0x1;
	uint32_t src_m=(src>>31)&0x1;
	uint32_t result_m=(result>>31)&0x1;
	if((dest_m!=src_m)&&(result_m==dest_m))
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

	cpu.SF=(result>>31)&0x1;

	cpu.esi+=incdec;
	cpu.edi+=incdec;

	print_asm("cmpsl");
	return 1;
}

make_helper_v(cmps)

//#include "cpu/exec/template-end.h"

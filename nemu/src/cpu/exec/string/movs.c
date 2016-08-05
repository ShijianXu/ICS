#include "cpu/exec/helper.h"
#include "movs.h"

//#define instr movsb
make_helper(movs_b) {
	int incdec;
	uint32_t temp=swaddr_read(cpu.esi,1,3);
	swaddr_write(cpu.edi,1,temp,0);
	if(cpu.DF==0)
		incdec=1;
	else
		incdec=-1;

	cpu.esi+=incdec;
	cpu.edi+=incdec;

	print_asm("movsb");
	return 1;
}
//make_instr_helper(movsb);
//#undef instr

//#define instr movsw

//make_helper_v(movs)

make_helper(movs_w) {
	int incdec;
	uint32_t temp=swaddr_read(cpu.esi,2,3);
	swaddr_write(cpu.edi,2,temp,0);
	if(cpu.DF==0)
		incdec=2;
	else
		incdec=-2;
	
	cpu.esi+=incdec;
	cpu.edi+=incdec;

	print_asm("movsw");
	return 1;
}
//make_instr_helper(movsw);
//#undef instr

//#define instr movsl
make_helper(movs_l) {
//	int incdec;
//	uint32_t temp=swaddr_read(cpu.esi,4);
	swaddr_write(cpu.edi,4,swaddr_read(cpu.esi,4,3),0);
/*	if(cpu.DF==0)
		incdec=4;
	else
		incdec=-4;
	
	cpu.esi+=incdec;
	cpu.edi+=incdec;
*/
	if(cpu.DF==0)
	{
		cpu.esi+=4;
		cpu.edi+=4;
	}
	else
	{
		cpu.esi-=4;
		cpu.edi-=4;
	}
	print_asm("movsl");
	return 1;
}
//make_instr_helper(movsl);
make_helper_v(movs)

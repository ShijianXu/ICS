#include "cpu/exec/helper.h"
#include "stos.h"

make_helper(stos_b) {
	int incdec;
	swaddr_write(cpu.edi,1,cpu.eax,0);
	if(cpu.DF==0)
		incdec=1;
	else
		incdec=-1;

//	cpu.esi+=incdec;
	cpu.edi+=incdec;

	print_asm("stosb");
	return 1;
}


make_helper(stos_w) {
	int incdec;
	swaddr_write(cpu.edi,2,cpu.eax,0);
	if(cpu.DF==0)
		incdec=2;
	else
		incdec=-2;
	
//	cpu.esi+=incdec;
	cpu.edi+=incdec;

	print_asm("stosw");
	return 1;
}


make_helper(stos_l) {
	int incdec;
	swaddr_write(cpu.edi,4,cpu.eax,0);
	if(cpu.DF==0)
		incdec=4;
	else
		incdec=-4;
	
//	cpu.esi+=incdec;
	cpu.edi+=incdec;

	print_asm("stosl");
	return 1;
}

make_helper_v(stos)

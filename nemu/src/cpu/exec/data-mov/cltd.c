#include "cpu/exec/helper.h"


make_helper(cltd) {

	if((int)cpu.eax<0)
		cpu.edx=0xffffffff;
	else
		cpu.edx=0;

	print_asm("cltd");
	return 1;
}

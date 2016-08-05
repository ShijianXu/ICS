#include "cpu/exec/helper.h"
#include "nemu.h"

make_helper(ljmp)
{
	cpu.eip = instr_fetch(eip+1,4);
	cpu.sreg[1].Sreg = instr_fetch(eip+5,2);
	
//	cpu.eip -= 7;

	print_asm("ljmp");

	return 0;
}

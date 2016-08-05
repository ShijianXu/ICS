#include "cpu/exec/helper.h"
#include "nemu.h"

make_helper(sti)
{
	cpu.IF = 1;
	print_asm("sti");
	return 1;
}

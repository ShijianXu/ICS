#include "cpu/exec/helper.h"
#include "nemu.h"

make_helper(std)
{
	cpu.DF = 1;
	print_asm("std");
	return 1;
}

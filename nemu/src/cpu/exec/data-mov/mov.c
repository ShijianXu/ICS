#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mov-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(mov_i2r)
make_helper_v(mov_i2rm)
make_helper_v(mov_r2rm)
make_helper_v(mov_rm2r)
make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)



/*
make_helper(mov_cr02r) {
	uint8_t index = instr_fetch(eip+1,1);
	REG(index) = cpu.cr0;
//  char cr0_name[10]="cr0";
//  print_asm("mov  %%%s,%%%s",cr0_name,REG_NAME(index);
	return 1;
}

make_helper(mov_r2cr0) {
	uint8_t index = instr_fetch(eip+1,1);
	cpu.cr0 = REG(index);
	//  char cr0_name[10]="cr0";
	//  print_asm("mov  %%%s,%%%s",REG_NAME(index),cr0_name);
	return 1;
}
*/

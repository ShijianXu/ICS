#include <setjmp.h>
#include "common.h"
#include "nemu.h"
extern jmp_buf jbuf;

void raise_intr(uint8_t NO) {
	/* TODO: Trigger an interrupt/exception with "NO".
	 * That is, use "NO" to index the IDT.
	 */
	
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.EFLAGS,2);

	cpu.IF=0;
	cpu.TF=0;
	
	cpu.esp-=4;
	swaddr_write(cpu.esp,2,cpu.sreg[1].Sreg,2);
	
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,cpu.eip,2);

	uint64_t intrdesc=lnaddr_read(cpu.IDTR.idt_addr + NO*8, 4);
	intrdesc = intrdesc+((uint64_t)lnaddr_read(cpu.IDTR.idt_addr + NO*8 +4,4)<<32);

	cpu.sreg[1].Sreg = (intrdesc & 0xffff0000) >> 16; 
	cpu.eip = ((intrdesc & 0xffff000000000000) >> 32) + (intrdesc&0xffff);

	/* Jump back to cpu_exec() */
	longjmp(jbuf, 1);
}

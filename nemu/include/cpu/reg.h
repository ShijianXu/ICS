#ifndef __REG_H__
#define __REG_H__

#include "common.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct SegmentDescriptor {
	uint32_t limit_15_0          : 16;
	uint32_t base_15_0           : 16;
	uint32_t base_23_16          : 8;
	uint32_t type                : 4;
	uint32_t segment_type        : 1;
	uint32_t privilege_level     : 2;
	uint32_t present             : 1;
	uint32_t limit_19_16         : 4;
	uint32_t soft_use            : 1;
	uint32_t operation_size      : 1;
	uint32_t pad0                : 1;
	uint32_t granularity         : 1;
	uint32_t base_31_24          : 8;
} SegDesc;

typedef struct Sreg_with_cache
{
	uint16_t Sreg;
	union
	{
		SegDesc Sreg_cache;
		uint64_t Sreg_cache_val;
	};
}SregCache;

typedef struct {
	union
	{
		union {
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		} gpr[8];

	/* Do NOT change the order of the GPRs' definitions. */
		struct
		{
			union
			{
				struct
				{uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;};
				struct
				{uint16_t ax,cx,dx,bx,sp,bp,si,di;};
			};
		};

	};
	
	union
	{
		struct
		{
			uint8_t CF	:1;
			uint8_t		:1;
			uint8_t PF	:1;
			uint8_t		:1;
			uint8_t AF	:1;
			uint8_t		:1;
			uint8_t ZF	:1;
			uint8_t SF	:1;
			uint8_t TF	:1;
			uint8_t IF	:1;
			uint8_t DF	:1;
			uint8_t OF	:1;
			uint8_t		:2;
			uint8_t	NT	:1;
			uint8_t		:1;
			uint8_t RF	:1;
			uint8_t VM	:1;
			uint16_t	:14;
		};
		uint32_t EFLAGS;
	};

	swaddr_t eip;
	
	struct
	{
		uint32_t gdt_addr;
		uint16_t limit;
	}GDTR;

	struct
	{
		uint32_t idt_addr;
		uint16_t limit;
	}IDTR;
	
	union
	{
		struct
		{
			uint32_t PE	: 1;
			uint32_t MP	: 1;
			uint32_t EM	: 1;
			uint32_t TS	: 1;
			uint32_t ET	: 1;
			uint32_t numeric_errot		: 1;
			uint32_t pad0				: 10;
			uint32_t write_protect		: 1;
			uint32_t pad1				: 1;
			uint32_t alignment_mask		: 1;
			uint32_t pad2				: 10;
			uint32_t no_write_through	: 1;
			uint32_t cache_disable		: 1;
			uint32_t PG	: 1;
		}CR0;
		uint32_t cr0;
	};
	
	union
	{
		struct
		{
			uint32_t pad0                : 3;
			uint32_t page_write_through  : 1;
			uint32_t page_cache_disable  : 1;
			uint32_t pad1                : 7;
			uint32_t page_directory_base : 20;
		}CR3;
		uint32_t cr3;
	};

	//00es 01cs 10ss 11ds
	
	SregCache sreg[4];

	volatile bool INTR;


/*	struct
	{
		uint16_t CS;
		union
		{
			SegDesc CS_cache;
			uint64_t CS_cache_val;
		};
	};
	struct
	{
		uint16_t DS;
		union
		{
			SegDesc DS_cache;
			uint64_t DS_cache_val;
		};
	};
	struct
	{
		uint16_t ES;
		union
		{
			SegDesc ES_cache;
			uint64_t ES_cache_val;
		};
	};
	struct
	{
		uint16_t SS;
		union
		{
			SegDesc SS_cache;
			uint64_t SS_cache_val;
		};
	};
*/	 
} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif

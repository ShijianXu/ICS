#include "common.h"
#include "burst.h"
#include "misc.h"
#include "mmu.h"

#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#define NR_ENTRY 64

typedef struct {
	bool valid;
	uint32_t tag;
	uint32_t frame;
}TLB;


TLB tlb[NR_ENTRY];


void init_tlb() {
	int i;
	for(i=0; i < NR_ENTRY; i++)
		tlb[i].valid = 0;
}

uint32_t tlb_read(lnaddr_t lnaddr, bool* hit)
{
	uint32_t tag = (lnaddr & 0xfffff000) >> 12;
	int i;
	for(i = 0; i < NR_ENTRY; i++)
	{
		if(tlb[i].valid==1 &&  (tag == tlb[i].tag))
		{
//printf("hit\n");
//			Log("hit vtag %05x -> frame %05x at %d\n",
//				tag, tlb[i].frame, i);
			return tlb[i].frame;
		}
	}
	*hit = false;
//	Log("miss vtag %05x", tag);
	return 0;
}

void tlb_write(lnaddr_t lnaddr, uint32_t frame)
{
	uint32_t tag = (lnaddr & 0xfffff000) >> 12;
	int i;
	for(i = 0; i < NR_ENTRY; i++)
	{
		if(tlb[i].valid == 0)
		{
			tlb[i].valid = 1;
			tlb[i].tag = tag;
			tlb[i].frame = frame;
			return;
		}
//		break;
	}
//printf("write miss\n");
	if(i == NR_ENTRY)
	{
		int index;

		srand((unsigned)time(NULL));
		index= ((unsigned)rand())%64;

//		Log("replace %d with vtag %05x, frame %05x", index, tag, frame);
		tlb[index].valid = 1;	
		tlb[index].tag = tag;
		tlb[index].frame = frame;
	}
}

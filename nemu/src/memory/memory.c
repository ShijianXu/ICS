#include "common.h"
#include "nemu.h"

#include "mmu.h"
uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t cache_1_read(hwaddr_t, size_t, bool*);
void cache_1_miss(hwaddr_t);
void cache_1_write(hwaddr_t, size_t, uint32_t);
uint32_t cache_2_read(hwaddr_t, size_t, bool*);
void cache_2_miss(hwaddr_t);
void cache_2_write(hwaddr_t, size_t, uint32_t);
uint32_t tlb_read(lnaddr_t, bool*);
void tlb_write(lnaddr_t, uint32_t);

/* Memory accessing interfaces */

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg)
{
	if(cpu.CR0.PE == 1)
	{
		lnaddr_t lnaddr=0;
		lnaddr=((cpu.sreg[sreg].Sreg_cache.base_31_24)<<24) + ((cpu.sreg[sreg].Sreg_cache.base_23_16)<<16) + cpu.sreg[sreg].Sreg_cache.base_15_0 + addr;
//Assert(addr==lnaddr,"%x     %x",addr,lnaddr);
		return lnaddr;
	}
	else
		return addr;
}

hwaddr_t page_translate(lnaddr_t addr)
{
	if(cpu.CR0.PE==1 && cpu.CR0.PG==1)
	{
		LNA lna;
		lna.addr_val = addr;

		bool hit=true;
		uint32_t temp = tlb_read(addr,&hit);
		if(hit)
			return (temp << 12) + lna.offset;
		else
		{
//Log("addr %x",addr);
			hwaddr_t dir_addr = (cpu.CR3.page_directory_base<<12) + lna.dir * 4;
			PDE pde;
			pde.val = hwaddr_read(dir_addr, 4);
		Assert(pde.present==1,"%x",addr);
	
			hwaddr_t page_addr = (pde.page_frame<<12) + lna.page * 4;
			PTE pte;
			pte.val = hwaddr_read(page_addr, 4);
		Assert(pte.present==1,"%x",addr);

	tlb_write(addr, pte.page_frame);

			hwaddr_t hwaddr = (pte.page_frame << 12) + lna.offset;
	
			return hwaddr;
		}
	}
	else
		return addr;
}

extern int is_mmio(hwaddr_t addr);
extern uint32_t mmio_read(hwaddr_t addr, size_t len, int map_NO);

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {	
	int io_no;
	io_no=is_mmio(addr);

	if(io_no != -1)
		return mmio_read(addr, len, io_no);
	else
	{
		bool hit = true;
		int offset = addr & 0x3f;
		if((offset+len)<64)
		{
			uint32_t temp = cache_1_read(addr, len, &hit);
			if(hit)
			{
				return temp;
			} 
			else
			{ 
				bool hit2 = true;
				uint32_t temp = cache_2_read(addr, len, &hit2);
				if(hit2)
				{
					cache_1_miss(addr);
					return temp;
				}
				else
				{
					temp = dram_read(addr,len) & (~0u >> ((4 - len) <<3));
					cache_2_miss(addr);
					return temp;
				}
			}
		}
		else
		{
			int i;
			uint32_t tp=0;
			uint32_t result=0;
			for(i = 0; i < len; i ++)
			{
				bool subhit=true;
				tp = cache_1_read(addr,1,&subhit);
				if(subhit)
				{
					tp = tp<<(i*8);
					result+=tp;
				}
				else
				{
					bool subhit2=true;
					tp=cache_2_read(addr,1,&subhit2);
					if(subhit2)
					{
						tp = tp<<(i*8);
						cache_1_miss(addr);
						result+=tp;
					}
					else
					{
						tp = dram_read(addr,1) & (~0u >> ((4 - 1) << 3));
						tp = tp<<(i*8);
						cache_2_miss(addr);
						result += tp;
					}
				}
				addr++;
			}
			 return result;
		}
	}	
}

extern void mmio_write(hwaddr_t addr, size_t len, uint32_t data, int map_NO);

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	int io_no;
	io_no=is_mmio(addr);
	if(io_no != -1)
		mmio_write(addr, len, data, io_no);
	else
	{	
		int i;
		for(i=0;i<len;i++)
		{
			uint8_t tp = data & 0xff;
			cache_1_write(addr,1,tp);
			data >>= 8;
			addr++;
		}
	}
}
/*
uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}*/

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	if(0==1) //special case, left to be handled later
		assert(0);
	else {
		hwaddr_t hwaddr = page_translate(addr);
		return hwaddr_read(hwaddr, len);
	}
}
/*
void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}*/

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	if(0==1)
		assert(0);
	else {
		hwaddr_t hwaddr = page_translate(addr);
		return hwaddr_write(hwaddr, len, data);
	}
}

/*
uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}
*/

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
	assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	return lnaddr_read(lnaddr, len);
}
/*
void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}*/

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
	assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	lnaddr_write(lnaddr, len, data);
}

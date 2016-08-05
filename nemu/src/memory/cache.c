#include "common.h"
#include "burst.h"
#include "misc.h"

#include "stdio.h"
#include "stdlib.h"
#include "time.h"

uint32_t dram_read(swaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
void cache_2_write(hwaddr_t addr, size_t len, uint32_t data);

/* tag:14 bit, cache组号:7 bit, 块内偏移地址:6 bit, tatal:27 bit */

/* The length of a slot is 64B */
#define SLOT 64
#define TAG_MASK 0x7ffe000
#define TAG_MASK_2 0x7fc0000
/* tag:9 bit, cache组号:12 bit, 块内偏移量:6 bit, total:27 bit */

typedef struct {
	bool valid;
	bool dirty;
	uint32_t tag;
	uint8_t slot[SLOT];
}cache;

/* 8-way set associative */
#define NR_WAY 8
#define NR_SET (1 << 7)
/* NR_ROW==NR_SET * NR-WAY */

/* 16-way set associative */
#define NR_WAY_2 16
#define NR_SET_2 (1 << 12)


cache cache_1[NR_SET][NR_WAY];
cache cache_2[NR_SET_2][NR_WAY_2];

void init_cache_1() {
	int i, j;
	for(i = 0; i < NR_SET; i ++) {
		for(j = 0; j < NR_WAY; j ++) {
			cache_1[i][j].valid = 0;
		}
	}
}

void init_cache_2() {
	int i,j;
	for(i = 0; i < NR_SET_2; i ++) {
		for(j = 0; j < NR_WAY_2; j ++) {
			cache_2[i][j].valid = 0;
			cache_2[i][j].dirty = 0;
		}
	}
}


uint32_t cache_1_read(hwaddr_t addr, size_t len, bool *hit)
{
	uint32_t block = (addr & 0x7ffffc0) >> 6;
	uint32_t set_num = block % NR_SET;
	int temp;
	int offset;
	int i;
	for(i = 0; i < NR_WAY; i ++)
	{
		temp = (addr & TAG_MASK) >> 13;
		if(temp == cache_1[set_num][i].tag && cache_1[set_num][i].valid == 1)
		{
			offset = addr & 0x3f;
			uint32_t result=0;
			int j;
			for(j = 0; j < len; j ++)
			{
				uint32_t tp=cache_1[set_num][i].slot[offset+j];
				tp=tp<<(j*8);
				result+=tp;
			} 
			return result;
		}
	}
	*hit = false;
	return 0;
}

uint32_t cache_2_read(hwaddr_t addr, size_t len, bool *hit)
{
	uint32_t block = (addr & 0x7ffffc0)>>6;
	uint32_t set_num = block % NR_SET_2;
	int temp;
	int offset;
	int i;
	for(i=0; i < NR_WAY_2; i ++)
	{
		temp = (addr & TAG_MASK_2)>>18;

		if(temp == cache_2[set_num][i].tag && cache_2[set_num][i].valid == 1)
		{
			offset = addr & 0x3f;
			uint32_t result=0;
			int j;
			for(j = 0; j < len; j ++)
			{
				uint32_t tp = cache_2[set_num][i].slot[offset+j];
				tp=tp<<(j*8);
				result += tp;
			}
			return result;
		}
	}
	*hit = false;
	return 0;
}

void cache_1_miss(hwaddr_t addr)
{
	uint32_t block = (addr & 0x7ffffc0) >> 6;
	uint32_t set_num_1 = block % NR_SET;
	uint32_t set_num_2 = block % NR_SET_2;
	int temp;
	int i;

	int way;
	int temp2;
	for(way = 0; way < NR_WAY_2; way ++)
	{
		temp2 = (addr & TAG_MASK_2)>>18;
		if(temp2 == cache_2[set_num_2][way].tag && cache_2[set_num_2][way].valid == 1)
			break;
	}
	for(i = 0; i < NR_WAY; i ++)
	{
		if(cache_1[set_num_1][i].valid == 0)
		{
			cache_1[set_num_1][i].valid = 1;
			temp=(addr & TAG_MASK) >> 13;
			cache_1[set_num_1][i].tag = temp;
			int j;
			for(j = 0; j < SLOT; j ++)
			{
				cache_1[set_num_1][i].slot[j]=cache_2[set_num_2][way].slot[j];
			}
			break;
		}	
	}
	if(i==NR_WAY)
	{
		int site;
		srand((unsigned)time(NULL));
		site=rand()%8;
		cache_1[set_num_1][site].valid=1;
		temp=(addr & TAG_MASK) >> 13;
		cache_1[set_num_1][site].tag = temp;
		int j;
		for(j = 0; j < SLOT; j ++)
		{
			cache_1[set_num_1][site].slot[j]=cache_2[set_num_2][way].slot[j];
		}
	}
}


void write_back(int set_num, int way)
{
	hwaddr_t addr=(cache_2[set_num][way].tag<<18)+(set_num<<6);
	int i;
	for(i=0;i<SLOT;i++)
	{
		dram_write(addr,1,cache_2[set_num][way].slot[i]);
		addr++;
	}
}



void cache_2_miss(hwaddr_t addr)
{
	uint32_t block = (addr & 0x7ffffc0)>>6;
	uint32_t set_num = block % NR_SET_2;
	hwaddr_t start = block << 6;
	int i;
	for(i = 0; i < NR_WAY_2; i ++)
	{
		if(cache_2[set_num][i].valid == 0)
		{
			cache_2[set_num][i].dirty=0;
			cache_2[set_num][i].valid=1;
			int temp=(addr & TAG_MASK_2) >> 18;
			cache_2[set_num][i].tag=temp;
			int j;
			for(j=0;j<SLOT;j++)
			{
				uint32_t tp=dram_read(start,1) & (~0u >> ((4 - 1) << 3));
				cache_2[set_num][i].slot[j]=tp;
				start++;
			}
			break;
		}
	}
	if(i == NR_WAY_2)
	{
		int site;
		srand((unsigned)time(NULL));
		site=rand()%16;

		if(cache_2[set_num][site].dirty==1)
		{
			write_back(set_num,site);
		}
		cache_2[set_num][site].dirty=0;
		cache_2[set_num][site].valid=1;
		int temp=(addr & TAG_MASK_2) >> 18;
		cache_2[set_num][site].tag=temp;
		int j;
		for(j = 0; j < SLOT; j ++)
		{
			cache_2[set_num][site].slot[j] = dram_read(start,1) & (~0u >> ((4 - 1) << 3));
			start++;
		}
	}
}

void cache_1_write(hwaddr_t addr, size_t len, uint32_t data)
{
	uint32_t block = (addr & 0x7ffffc0)>>6;
	uint32_t set_num = block % NR_SET;
	int temp;
	int offset;
	int i;
	for(i = 0; i < NR_WAY; i ++)
	{
		temp = (addr & TAG_MASK) >> 13;
		if(temp == cache_1[set_num][i].tag && cache_1[set_num][i].valid == 1)
		{
			offset = addr & 0x3f;
			cache_1[set_num][i].slot[offset]=data;
			break;
		}
	}
	cache_2_write(addr,len,data);
}

void cache_2_write(hwaddr_t addr, size_t len, uint32_t data)
{
	uint32_t block = (addr & 0x7ffffc0)>>6;
	uint32_t set_num = block % NR_SET_2;
	int temp;
	int offset;
	int i;
	for(i = 0; i < NR_WAY_2; i ++)
	{
		temp = (addr & TAG_MASK_2) >> 18;
		if(temp == cache_2[set_num][i].tag && cache_2[set_num][i].valid == 1)
		{
			offset = addr & 0x3f;
			cache_2[set_num][i].slot[offset]=data;
			cache_2[set_num][i].dirty = 1;
			break;
		}
	}
	
	if(i==NR_WAY_2)
	{
		dram_write(addr,len,data);
		cache_2_miss(addr);
	}
}

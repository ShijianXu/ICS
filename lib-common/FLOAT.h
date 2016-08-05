#ifndef __FLOAT_H__
#define __FLOAT_H__

#include "trap.h"

typedef int FLOAT;

static inline int F2int(FLOAT a) {
	int result;
	int s=(a>>31)&0x1;
	if(s==0)
	{
		int temp=a&0x7fffffff;
		result=temp/(1<<16);
	}
	else
	{
		int temp=(~(a&0x7fffffff))+1;
		result=-temp/(1<<16);
	}
//	nemu_assert(0);
	return result;
}

static inline FLOAT int2F(int a) {
	int ta;
	if(a<0)
		ta=-a;
	else
		ta=a;
	FLOAT result=ta*(1<<16);
	if(a<0)
		result=-result;
//	nemu_assert(0);
	return result;
}

static inline FLOAT F_mul_int(FLOAT a, int b) {
	FLOAT result=a*b;
//	nemu_assert(0);
	return result;
}

static inline FLOAT F_div_int(FLOAT a, int b) {
	FLOAT result=a/b;
//	nemu_assert(0);
	return result;
}

FLOAT f2F(float);
FLOAT F_mul_F(FLOAT, FLOAT);
FLOAT F_div_F(FLOAT, FLOAT);
FLOAT Fabs(FLOAT);
FLOAT sqrt(FLOAT);
FLOAT pow(FLOAT, FLOAT);

#endif

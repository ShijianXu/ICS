#include "FLOAT.h"
/*
typedef signed short   int16;
typedef signed int     int32;
typedef long long	   int64;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef unsigned long long uint64;

uint16 uint64_div_uint32(uint64 a,uint32 b,uint64* addr3,uint32* addr4) {
	union{
		uint64 un[2];
		struct{
			uint32 l0;
			uint32 h0;
			uint32 l1;
			uint32 h1;
		}st;
	}aa;
	uint64 cc;
	register uint32 i;

	if(b==0) return 1;
	if(a<b){
		*addr3=0;
		*addr4=a;
		return 0;
	}
	aa.un[0]=a;
	aa.un[1]=0;
	cc=0;
	i=0;

	if(aa.st.h0==0) {
		aa.st.h0=aa.st.l0;
		aa.st.l0=0;
		i=32;
	}
//nemu_assert(0);
	for(;i<64;++i){
		if((aa.st.h0&0x80000000)==0x80000000)
				 break;
		else 
		{
			//	nemu_assert(0);
				aa.un[0]<<=1;
		}
	}
	for(;i<64;++i){
		aa.un[1]=(aa.un[1]<<1)+(aa.un[0]>>63);
		aa.un[0]<<=1;
		cc=cc<<1;
		if(aa.un[1]>=b){
			aa.un[1]=aa.un[1]-b;
			++cc;
		}
	}
	*addr3=cc;
	*addr4=aa.st.l1;
	return 0;
}
*/

FLOAT F_mul_F(FLOAT a, FLOAT b) {	
	long long temp=((long long)a*(long long)b)&0xffffffff0000;
	FLOAT result=temp>>16;
//	nemu_assert(0);
	return result;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
//	FLOAT result=(a/b)*(1<<16);
/*	uint64 la=a;
	uint32 lb=b;
	uint64 addr3=0;
	uint32 addr4=0;
	uint64_div_uint32(la,lb,&addr3,&addr4);

	uint32 result=((addr3<<16)&0xffff0000)+(addr4&0xffff);
//	nemu_assert(0);
	return (FLOAT)result;

	*///已廢棄

  //不確定
	int la=a<<13;
	int lb=b>>3;
	int result=la/lb;
	return (FLOAT)(result);

//	FLOAT result=a/b;
//	FLOAT c=a-b*result;
	   
}

FLOAT f2F(float a) {
	int temp=*(int *)&a;
	int exp=((temp&0x7f800000)>>23)-127;
	int sign=(temp>>31)&0x1;
	FLOAT result=(temp&0x7fffff)|0x800000;
	
	int shift=exp+16;
	shift=23-shift;
	if(shift>0)
		result>>=shift;
	else if(shift<0)
		result<<=shift;
	if(sign==1)
		result=-result;
//	nemu_assert(0);
	return result;
}

FLOAT Fabs(FLOAT a) {
	FLOAT result;
	int s=a>>31;
	if(s==0)
		result=a;
	else
		result=-a;
//	nemu_assert(0);
	return result;
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}


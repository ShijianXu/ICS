#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

#include<stdlib.h>

extern uint32_t find_addr(char *val,bool *is_obj);
bool expr_ok=true;   //a global val
bool reg_right=true;
bool is_obj=true;


/***********************change*********************************/
enum {
	NOTYPE = 256, EQ, NUMBER, VAR, HEX, SREG, REG, EFLAGS, NEQ, LOGAND, LOGOR, LOGNOT, DEREF
	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"\\-", '-'},					// minus						
	{"\\*", '*'},					// multiply | derefference
	{"\\/", '/'},					// divide
	{"\\(", '('},					// l_bracket
	{"\\)", ')'},					// r_bracket
	{"([a-zA-Z]|_)([a-zA-Z0-9]|_)*", VAR}, // variable
	{"0[xX][0-9a-fA-F]+", HEX}, 	// hex
	{"\\$[CDES]S", SREG},				// SREG
	{"\\$[a-z]{3}", REG},			// reg
	{"\\$[OSZAPC]F", EFLAGS},		// eflags
	{"[0-9]+", NUMBER},				// numbers
	{"==", EQ},						// equal
	{"!=", NEQ},					// not equal
	{"&&", LOGAND},					// logic and
	{"\\|\\|", LOGOR},					// logic or
	{"!", LOGNOT},					// logic not
};
/*********************************************************************/

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
//				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */
/************from here, the code is mine*********************/
				switch(rules[i].token_type) {
					case NOTYPE:break;
					case '+':case '-':case '*':case '/':case '(':case ')':case EQ:case NEQ:case LOGAND:case LOGOR:case LOGNOT:
						tokens[nr_token].type=rules[i].token_type;nr_token++;break;
					case NUMBER:case HEX:case SREG:case REG:case EFLAGS:case VAR:				
						tokens[nr_token].type=rules[i].token_type;
						char substring[32];
						strcpy(substring,substr_start);
						substring[substr_len]='\0';
						strcpy(tokens[nr_token].str,substring);
						nr_token++;
						break;
				
					default: panic("please implement me");
				}
/************************************************************/
			break;
			}
		}
		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");	
			return false;
		}
	}


	return true; 
}

bool check_parentheses(int p,int q)
{
//printf("%s %s\n",tokens[p].str,tokens[q].str);
	int count=0;
	int i;
	for(i=p;i<=q;i++)
	{
		if(count<0)
		{
//			printf("lack of left parenthese!\n");
			return false;
		}
		if(tokens[i].type=='(')
			count++;
		else if(tokens[i].type==')')
			count--;
	}
	if(count!=0)
		return false;
	return true;
}

int op_priority(int i)
{
	if(tokens[i].type==LOGNOT)
		return 1;
	else if(tokens[i].type=='*' || tokens[i].type=='/')
		return 2;
	else if(tokens[i].type=='+' || tokens[i].type=='-')
		return 3;
	else if(tokens[i].type==EQ ||tokens[i].type==NEQ)
		return 4;
	else if(tokens[i].type==LOGAND || tokens[i].type==LOGOR)
		return 4;
//	assert(0);
return 0;
}

int find_domiop(int p,int q)
{
	int domiop_ip=0;
	int count=0;
	int i;
	for(i=p;i<=q;i++)
	 {
		if(tokens[i].type==VAR || tokens[i].type==NUMBER || tokens[i].type==HEX || tokens[i].type==REG)
			continue;
		else if(tokens[i].type=='(')
			count++;
		else if(tokens[i].type==')')
			count--;
		else if(domiop_ip==0 && count==0)
			domiop_ip=i;
		else if(op_priority(i)>=op_priority(domiop_ip) && count==0)
				domiop_ip=i;
	} 
	return domiop_ip;
}

bool check_legal(int p,int q)
{
	if(tokens[p].type=='(' && tokens[q].type==')' && check_parentheses(p+1,q-1)==true)
		return true;
	else
		return false;
}

uint32_t eval(int p,int q)
{
	expr_ok=true;

	int dominant=0;
//printf("%d %d\n",p,q);
	if(p>q){
		printf("This is a bad expression\n");
		expr_ok=false;
		return 0;
	}
	else if(p==q){
		if(tokens[p].type==NUMBER)
			return atoi(tokens[p].str);
		else if(tokens[p].type==HEX)
		{
			char *hexnum=tokens[p].str+2;
			uint32_t sum=0,temp=0;
			int i;
			for(i=0;hexnum[i]!='\0';i++)
			{
				if(hexnum[i]>='0' && hexnum[i]<='9')
					temp=hexnum[i]-'0';
				else if(hexnum[i]>='a' && hexnum[i]<='f')
					temp=hexnum[i]-'a'+10;
				sum=sum*16+temp;
			}
			return sum; 
		}

		/*************** VAR NOT DONE **********************/
		else if(tokens[p].type==VAR)
		{
			uint32_t addr=0;
			addr=find_addr(tokens[p].str,&is_obj);
			if(!is_obj)
				printf("NO SUCH OBJECT!\n");
			return addr;
//printf("test %2x\n",addr);
		}
		/*************** VAR NOT DONE **********************/

		else if(tokens[p].type==REG)
		{

			reg_right=true;

			if(strcmp(tokens[p].str,"$eax")==0)
				return cpu.eax;
			else if(strcmp(tokens[p].str,"$ecx")==0)
				return cpu.ecx;
			else if(strcmp(tokens[p].str,"$edx")==0)
				return cpu.edx;
			else if(strcmp(tokens[p].str,"$ebx")==0)
				return cpu.ebx;
			else if(strcmp(tokens[p].str,"$esp")==0)
				return cpu.esp;
			else if(strcmp(tokens[p].str,"$ebp")==0)
				return cpu.ebp;
			else if(strcmp(tokens[p].str,"$esi")==0)
				return cpu.esi;
			else if(strcmp(tokens[p].str,"$edi")==0)
				return cpu.edi;
			else if(strcmp(tokens[p].str,"$eip")==0)
				return cpu.eip;

			else 
			{
				printf("NO SUCH REGISTER!\n");
				expr_ok=false;
				reg_right=false;
				return 0;
			}
		}
		else if(tokens[p].type==SREG)
		{
			if(strcmp(tokens[p].str,"$ES")==0)
				return cpu.sreg[0].Sreg;
			else if(strcmp(tokens[p].str,"$CS")==0)
				return cpu.sreg[1].Sreg;
			else if(strcmp(tokens[p].str,"$SS")==0)
				return cpu.sreg[2].Sreg;
			else if(strcmp(tokens[p].str,"$DS")==0)
				return cpu.sreg[3].Sreg;
			else
			{
				printf("A Bad expression!\n");
				expr_ok=false;
				return 0;
	        }
		}
		else if(tokens[p].type==EFLAGS)
		{
			if(strcmp(tokens[p].str,"$OF")==0)
				return cpu.OF;
			else if(strcmp(tokens[p].str,"$SF")==0)
				return cpu.SF;
			else if(strcmp(tokens[p].str,"$ZF")==0)
				return cpu.ZF;
			else if(strcmp(tokens[p].str,"$AF")==0)
				return cpu.AF;
			else if(strcmp(tokens[p].str,"$PF")==0)
				return cpu.PF;
			else if(strcmp(tokens[p].str,"$CF")==0)
				return cpu.CF;
		}
		else
		{
			printf("A Bad expression!\n");
			expr_ok=false;
			return 0;
		}
	}
	bool check=check_parentheses(p,q);
	if(!check)
	{
		printf("The parentheses not match!\n");
		expr_ok=false;
		return 0;
	}
	else if(check)
	{
			bool legal;
			legal=check_legal(p,q);
		if(legal)
			return eval(p+1,q-1);		
		else
		{
			dominant=find_domiop(p,q);
			if(tokens[dominant].type==LOGNOT)
			{
				int val3=eval(dominant+1,q);
				return !val3;
			}
			else if(tokens[dominant].type==DEREF)
			{
				int val4=eval(dominant+1,q);
				uint32_t value=swaddr_read(val4,4,3);
				return value;
			}
			else
			{		
				int val1=eval(p,dominant-1);
				if(expr_ok==false)
					return 0;
				int val2=eval(dominant+1,q);
				if(expr_ok==false)
					return 0;
				switch(tokens[dominant].type)
				{
					case '+':return val1+val2;
					case '-':return val1-val2;
					case '*':return val1*val2;
					case '/':return val1/val2;
					case EQ:return val1==val2;
					case NEQ:return val1!=val2;
					case LOGAND:return val1&&val2;
					case LOGOR:return val1||val2;
					default: assert(0);
				}
			}
		}

	}

	return 0;
}

bool Isop(int i)
{
	if(tokens[i].type=='(' || tokens[i].type=='+' || tokens[i].type=='-' || tokens[i].type=='*' || tokens[i].type=='/' || tokens[i].type==EQ || tokens[i].type==NEQ || tokens[i].type==LOGAND || tokens[i].type==LOGOR)
		return true;
	else 
		return false;
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	/* TODO: Insert codes to evaluate the expression. */

	int i;
	for(i=0;i<nr_token;i++)
	{
		if(tokens[i].type=='*' && (i==0 || Isop(i-1)))
			tokens[i].type=DEREF;
	}

	int p=0,q=nr_token-1;
	uint32_t result=eval(p,q);
//printf("test 0x%2x\n",result);
	if(!reg_right)
	{
		*success=false;
		return 0;
	}
	if(!is_obj)
	{
		*success=false;
		return 0;
	}

	return result;
}



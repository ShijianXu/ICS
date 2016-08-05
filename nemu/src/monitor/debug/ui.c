#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct {
	swaddr_t prev_ebp;
	swaddr_t ret_addr;
	uint32_t args[4];
}PartOfStackFrame;

PartOfStackFrame psf;

void cpu_exec(uint32_t);

extern void in_which_func(uint32_t addr);
uint32_t cache_1_read(hwaddr_t addr, size_t len, bool *hit);


void print_frame_list() {
	int count=0;
	psf.prev_ebp=cpu.ebp;
	psf.ret_addr=cpu.eip;
	while(psf.prev_ebp!=0)//已處理，判斷eip是否等於0x100000,不是就可以執行
	{
		printf("#%d ",count);
		count++;
		in_which_func(psf.ret_addr);
		psf.ret_addr=swaddr_read(psf.prev_ebp+4,4,2);
				
		psf.args[0]=swaddr_read(psf.prev_ebp+4,4,2);
		psf.args[1]=swaddr_read(psf.prev_ebp+8,4,2);
		psf.args[2]=swaddr_read(psf.prev_ebp+12,4,2);
		psf.args[3]=swaddr_read(psf.prev_ebp+16,4,2);

		psf.prev_ebp=swaddr_read(psf.prev_ebp,4,2);
		
		printf("ret_addr\t0x%2x\n",psf.ret_addr);
		printf("0x%2x\t",psf.args[0]);
		printf("0x%2x\n",psf.args[1]);
		printf("0x%2x\t",psf.args[2]);
		printf("0x%2x\n",psf.args[3]);
	}
}





/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args) {
	if (args==NULL)
		cpu_exec(1);
	else
	{
		int n=atoi(args);
		cpu_exec(n);
	}
	return 0;
}

static int cmd_info(char *args) {
	if (args==NULL)
		printf("lack of exact instruction\n");
	else if (strcmp(args,"r")==0)
	{
		printf("%s\t0x%08x\n","eax",cpu.eax);
		printf("%s\t0x%08x\n","ecx",cpu.ecx);
		printf("%s\t0x%08x\n","edx",cpu.edx);
		printf("%s\t0x%08x\n","ebx",cpu.ebx);
		printf("%s\t0x%08x\n","esp",cpu.esp);
		printf("%s\t0x%08x\n","ebp",cpu.ebp);
		printf("%s\t0x%08x\n","esi",cpu.esi);
		printf("%s\t0x%08x\n","edi",cpu.edi);
		printf("%s\t0x%08x\n","eip",cpu.eip);
		printf("%s\t0x%d\n","CF",cpu.CF);
		printf("%s\t0x%d\n","OF",cpu.OF);
		printf("%s\t0x%d\n","ZF",cpu.ZF);
		printf("%s\t0x%d\n","PF",cpu.PF);
		printf("%s\t0x%d\n","SF",cpu.SF);

		printf("%s\t0x%02x\n","CS",cpu.sreg[1].Sreg);
		printf("%s\t0x%02x\n","SS",cpu.sreg[2].Sreg);
		printf("%s\t0x%02x\n","ES",cpu.sreg[0].Sreg);
		printf("%s\t0x%02x\n","DS",cpu.sreg[3].Sreg);

		printf("%s\t0x%08x\n","CR0",cpu.cr0);
		printf("%s\t0x%d\n","CR0.PE",cpu.CR0.PE);
		printf("%s\t0x%d\n","CR0.PG",cpu.CR0.PG);
		printf("%s\t0x%08d\n","CR3",cpu.cr3);
		printf("%s\t0x%08x\n","GDTR.gdt_addr",cpu.GDTR.gdt_addr);
		printf("%s\t0x%02x\n","GDTR.limit",cpu.GDTR.limit);

		printf("%s\t0x%08x\n","IDTR.idt_addr",cpu.IDTR.idt_addr);
		printf("%s\t0x%02x\n","IDTR.limit",cpu.IDTR.limit);
	}
	else if(strcmp(args,"w")==0) 
//		printf("haven't done!\n");
/*****************test*******************/		
	{
		print_list();
	}
	return 0;
}

static int cmd_p(char *args) {
	if(args==NULL)
	{
		printf("Lack of sub-command!\n");
		return 0;
	}
	bool ok=true;
	uint32_t result=expr(args,&ok);
	if(ok==true)
		printf("> 0x%2x\n",result);
	return 0;	
}

static int cmd_bt()
{
	if(cpu.eip<0x800000)
		printf("No stack.\n");
	else
		print_frame_list();	
	return 0;
}

static int cmd_x(char *args) {
	char *arg = strtok(NULL," ");
	if(arg==NULL)
	{
		printf("%s\n","Instruction is not right!");
		return 0;
	}
	int c=atoi(arg);

	arg = strtok(NULL," ");
	
	int addr=strtod(arg,NULL);
printf("addr is %d\n",addr);
	int i=0;

	for(;i<c;i++)
	{
		uint32_t value=swaddr_read(addr+4*i,4,3);
		printf("%08x\n",value);
	}
	return 0;		
}

static int cmd_w(char *args)
{
	if(args==NULL)
	{
		printf("Lack of subcommand \n");
		return 0;
	}
	new_wp(args);

	return 0;
}

static int cmd_cache(char *args)
{
	bool hit=true;
	uint32_t addr=strtod(args,NULL);
	cache_1_read(addr,4,&hit);
	if(hit)
	{
		uint32_t block = (addr & 0x7ffffc0)>>6;
		uint32_t set_num = block % (1<<7);
		printf("block:%d\n",block);
		printf("set_num:%d\n",set_num);
	}
	else
	{
		printf("MISS!\n");
	}
	return 0;
}

static int cmd_d(char *args)
{
	if(args==NULL)
	{
		printf("DELETE WHICH WATCHPOINT?\n");
		return 0;
	}

	int i=atoi(args);

	delete_watch(i);
	return 0;
}

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Single step", cmd_si },
	{ "info", "Print status", cmd_info },
	{ "p", "Compute expression", cmd_p },
	{ "x", "Examine memory", cmd_x},
	{ "w", "Set watch point", cmd_w},
	{ "d", "Delete a watch point", cmd_d},
	{ "bt", "Print the stack frame list", cmd_bt},
	{ "cache", "Check an address in cache", cmd_cache}
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}

#include "nemu.h"

#define ENTRY_START 0x100000

extern uint8_t entry [];
extern uint32_t entry_len;
extern char *exec_file;

void load_elf_tables(int, char *[]);
void init_regex();
void init_wp_list();
void init_ddr3();
void init_cache_1(); //set valid bit to 0
void init_cache_2();
void init_tlb();
void init_device();
void init_sdl();

FILE *log_fp = NULL;

static void init_log() {
	log_fp = fopen("log.txt", "w");
	Assert(log_fp, "Can not open 'log.txt'");
}

static void welcome() {
	printf("Welcome to NEMU!\nThe executable is %s.\nFor help, type \"help\"\n",
			exec_file);
}

void init_monitor(int argc, char *argv[]) {
	/* Perform some global initialization */

	/* Open the log file. */
	init_log();

	/* Load the string table and symbol table from the ELF file for future use. */
	load_elf_tables(argc, argv);

	/* Compile the regular expressions. */
	init_regex();

	/* Initialize the watchpoint link list. */
	init_wp_list();

	/* Initialize the device and sdl  PA4 */
#ifdef HAS_DEVICE
	init_device();
	init_sdl();
#endif
	/* Display welcome message. */
	welcome();
}

#ifdef USE_RAMDISK
static void init_ramdisk() {
	int ret;
	const int ramdisk_max_size = 0xa0000;
	FILE *fp = fopen(exec_file, "rb");
	Assert(fp, "Can not open '%s'", exec_file);

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	Assert(file_size < ramdisk_max_size, "file size(%zd) too large", file_size);

	fseek(fp, 0, SEEK_SET);
	ret = fread(hwa_to_va(0), file_size, 1, fp);
	assert(ret == 1);
	fclose(fp);
}
#endif

static void load_entry() {
	int ret;
	FILE *fp = fopen("entry", "rb");
	Assert(fp, "Can not open 'entry'");

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);

	fseek(fp, 0, SEEK_SET);
	ret = fread(hwa_to_va(ENTRY_START), file_size, 1, fp);
	assert(ret == 1);
	fclose(fp);
}

void restart() {
	/* Perform some initialization to restart a program */
#ifdef USE_RAMDISK
	/* Read the file with name `argv[1]' into ramdisk. */
	init_ramdisk();
#endif

	/* Read the entry code into memory. */
	load_entry();

	/* Set the initial instruction pointer. */
	cpu.eip = ENTRY_START;
/*i added the codes here*/
	/* Set the initial eflags*/
	cpu.CF = 0;
	cpu.PF = 0;
	cpu.AF = 0;
	cpu.ZF = 0;
	cpu.SF = 0;
	cpu.TF = 0;
	cpu.IF = 0;
	cpu.DF = 0;
	cpu.OF = 0;
	cpu.NT = 0;
	cpu.RF = 0;
	cpu.VM = 0; 
	/* Set the initial CR0 */
	cpu.CR0.PE = 0;
	cpu.CR0.MP = 0;
	cpu.CR0.EM = 0;
	cpu.CR0.TS = 0;
	cpu.CR0.ET = 0;
	cpu.CR0.PG = 0;
	
	cpu.sreg[1].Sreg_cache.limit_15_0 = 0xffff;
	cpu.sreg[1].Sreg_cache.limit_19_16 = 0xf;
	cpu.sreg[1].Sreg_cache.base_15_0 = 0x0;
	cpu.sreg[1].Sreg_cache.base_23_16 = 0x0;
	cpu.sreg[1].Sreg_cache.base_31_24 = 0x0;

	cpu.sreg[0].Sreg_cache.limit_15_0 = 0xffff;
	cpu.sreg[0].Sreg_cache.limit_19_16 = 0xf;
	cpu.sreg[0].Sreg_cache.base_15_0 = 0x0;
	cpu.sreg[0].Sreg_cache.base_23_16 = 0x0;
	cpu.sreg[0].Sreg_cache.base_31_24 = 0x0;


	cpu.sreg[2].Sreg_cache.limit_15_0 = 0xffff;
	cpu.sreg[2].Sreg_cache.limit_19_16 = 0xf;
	cpu.sreg[2].Sreg_cache.base_15_0 = 0x0;
	cpu.sreg[2].Sreg_cache.base_23_16 = 0x0;
	cpu.sreg[2].Sreg_cache.base_31_24 = 0x0;

	cpu.sreg[3].Sreg_cache.limit_15_0 = 0xffff;
	cpu.sreg[3].Sreg_cache.limit_19_16 = 0xf;
	cpu.sreg[3].Sreg_cache.base_15_0 = 0x0;
	cpu.sreg[3].Sreg_cache.base_23_16 = 0x0;
	cpu.sreg[3].Sreg_cache.base_31_24 = 0x0;


	/* Initialize DRAM. */
	init_ddr3();

/***********************************************/
	/* Initializa cache. */
	init_cache_1();
	init_cache_2();
	init_tlb();
}

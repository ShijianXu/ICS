#define PAGE_SIZE                   4096
#define NR_PDE                      1024
#define NR_PTE                      1024
#define PAGE_MASK                   (4096 - 1)
#define PT_SIZE                     ((NR_PTE) * (PAGE_SIZE))

typedef union LNADDR {
	struct {
		uint32_t offset	: 12;
		uint32_t page	: 10;
		uint32_t dir	: 10;
	};
	uint32_t addr_val;
} LNA;

typedef union PageDirectoryEntry {
	struct {
		uint32_t present             : 1;
		uint32_t read_write          : 1;
		uint32_t user_supervisor     : 1;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t accessed            : 1;
		uint32_t pad0                : 6;
		uint32_t page_frame          : 20;
	};
	uint32_t val;
} PDE;

/* the 32bit Page Table Entry(second level page table) data structure */
typedef union PageTableEntry {
	struct {
		uint32_t present             : 1;
		uint32_t read_write          : 1;
		uint32_t user_supervisor     : 1;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t accessed            : 1;
		uint32_t dirty               : 1;
		uint32_t pad0                : 1;
		uint32_t global              : 1;
		uint32_t pad1                : 3;
		uint32_t page_frame          : 20;
	};
	uint32_t val;
} PTE;

typedef PTE (*PT) [NR_PTE];

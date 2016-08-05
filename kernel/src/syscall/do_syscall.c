#include "irq.h"

#include <sys/syscall.h>

extern int fs_open(const char *pathname);
extern int fs_read(int fd, void *buf, int len);
extern int fs_write(int fd, void *buf, int len);
extern int fs_lseek(int fd, int offset, int whence);
extern int fs_close(int fd);

extern void serial_printc(char ch);

void add_irq_handle(int, void (*)(void));
void mm_brk(uint32_t);

static void sys_brk(TrapFrame *tf) {
#ifdef IA32_PAGE
	mm_brk(tf->ebx);
#endif
	tf->eax = 0;
}

void do_syscall(TrapFrame *tf) {
	switch(tf->eax) {
		/* The ``add_irq_handle'' system call is artificial. We use it to 
		 * let user program register its interrupt handlers. But this is 
		 * very dangerous in a real operating system. Therefore such a 
		 * system call never exists in GNU/Linux.
		 */
		case 0: 
			cli();
//	Log("tf->ebx %x",tf->ebx);
			add_irq_handle(tf->ebx, (void*)tf->ecx);
			sti();
			break;

		case SYS_brk:
		{
			 sys_brk(tf); 
			 break;
		}

		/* TODO: Add more system calls. */		
		case SYS_write:
		{
/*			if(tf->ebx==1)
			{
//				asm volatile (".byte 0xd6" : : "a"(SYS_write), "c"(tf->ecx), "d"(tf->edx));
				int i;
				for(i=0; i<tf->edx; i++)
				{
					serial_printc(*(char *)(tf->ecx + i));
				}

				tf->eax = tf->edx;
				break;
			}
			else
				break;*/
			uint32_t buf = tf->ecx;
			uint32_t len = tf->edx;
			if(tf->ebx == 1 || tf->ebx == 2)
			{
				int cnt =0;
				while(cnt < len)
				{
					serial_printc(*((char*)buf++));
					cnt++;
				}
				tf->eax = len;
			}
			else if(tf->ebx >=3)
				tf->eax = fs_write(tf->ebx, (void*)buf, len);
			else
				tf->eax = -1;
			break;
		}

		case SYS_open:
		{
			
			tf->eax = fs_open((char*)tf->ebx);
			break;
		}

		case SYS_read:
		{
			tf->eax = fs_read(tf->ebx, (void*)tf->ecx, tf->edx);
			break;
		}

		case SYS_lseek:
		{
			tf->eax = fs_lseek(tf->ebx, tf->ecx, tf->edx);
			break;
		}

		case SYS_close:
		{
			tf->eax = fs_close(tf->ebx);
			break;
		}
		default: panic("Unhandled system call: id = %d", tf->eax);
	}
}


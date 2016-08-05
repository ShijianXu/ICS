#include "common.h"
#include "string.h"

typedef struct {
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

enum {SEEK_SET, SEEK_CUR, SEEK_END};

/* This is the information about all files in disk. */
static const file_info file_table[] = {
	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582}, {"fbp.mkf", 1128064, 3232609},
	{"fire.mkf", 834728, 4360673}, {"f.mkf", 186966, 5195401},
	{"gop.mkf", 11530322, 5382367}, {"map.mkf", 1496578, 16912689},
	{"mgo.mkf", 1577442, 18409267}, {"m.msg", 188232, 19986709},
	{"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);

/* TODO: implement a simplified file system here. */

typedef struct {
	bool opened;
	uint32_t offset;
} Fstate;

Fstate file_state[NR_FILES + 3];

int fs_open(const char *pathname)
{
//	Log("hit open");
	bool found = false;
	int i = 0;
	for(; i < NR_FILES; i++)
	{
		if(strcmp(file_table[i].name, pathname)==0)
		{
			found = true;
			break;
		}
	}
	if(found)
	{
		file_state[i+3].opened = true;
		file_state[i+3].offset = 0;
		return i+3;
	}
	else
		assert(0);
	return 0;
}

int fs_read(int fd, void *buf, int len)
{
	if(len < 0)
		return -1;
	else if(fd == 0 || fd == 1 || fd == 2) // || fd > (NR_FILES + 3))
		//assert(0);
		return -1;
	else
	{
		assert(file_state[fd].opened);
		if(file_table[fd-3].size == file_state[fd].offset)
			return 0;
		if(len > file_table[fd-3].size - file_state[fd].offset)
			len = file_table[fd-3].size - file_state[fd].offset;

		ide_read(buf, file_table[fd-3].disk_offset + file_state[fd].offset, len);
		file_state[fd].offset += len;
		return len;
	}
}

extern void serial_printc(char ch);
		 
int fs_write(int fd, void *buf, int len)
{ 
	if(len < 0)
		return -1;
	else if(fd < 3)
	{
		return -1;
	}
	else
	{
		assert(file_state[fd].opened);
		if(len > file_table[fd-3].size - file_state[fd].offset)
			len = file_table[fd-3].size - file_state[fd].offset;
		ide_write(buf, file_table[fd-3].disk_offset + file_state[fd].offset, len);
		file_state[fd].offset += len;
		return len;
	}
}
/*
int fs_lseek(int fd, int offset, int whence)
{
	assert(fd >= 3);
	assert(file_state[fd].opened);
	if(whence == SEEK_SET)
	{
		if(offset < 0 || offset >= file_table[fd - 3].size)
			return -1;
		file_state[fd].offset = offset;
		return file_state[fd].offset;
	}
	else if(whence == SEEK_CUR)
	{
		if(file_state[fd].offset + offset < 0 || file_state[fd].offset + offset >= file_table[fd -3].size)
			return -1;
		file_state[fd].offset += offset;
		return file_state[fd].offset;
	}
	else if(whence == SEEK_END)
	{
		if(offset > 0 || offset >= -(file_table[fd - 3].size))
			return -1;
		file_state[fd].offset = file_table[fd-3].size + offset;
		return file_state[fd].offset;
	}
	else
		return -1;
}*/
int fs_lseek(int fd, int offset, int whence){
	if (fd == 0 || fd == 1 || fd == 2)
		assert(0);
	assert(file_state[fd].opened);
	int size=file_table[fd-3].size;
	int offset_=0;
	if (whence == SEEK_SET)
		offset_=0;
	else if (whence == SEEK_CUR)
		offset_=file_state[fd].offset;
	else if (whence == SEEK_END)
		offset_=file_table[fd-3].size;
	else
		assert(0);
	int temp=offset_+offset;
	if (temp<0)
		temp=0;
	if (temp>size)
		file_state[fd].offset=(size-1);
	else
		file_state[fd].offset=temp;
	return file_state[fd].offset;
}

int fs_close(int fd)
{
	if(fd == 1 || fd == 2)
		assert(0);
	if(!file_state[fd].opened)
		return -1;
	file_state[fd].opened = false;
	file_state[fd].offset = 0;
	return 0;
}

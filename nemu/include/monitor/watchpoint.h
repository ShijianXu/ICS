#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	char exp[32];
	uint32_t value;
	/* TODO: Add more members if necessary */
//frome here	

} WP;

#endif



WP* new_wp(char *arg);

void free_wp(WP *wp);

void delete_watch(int i);

void print_list();

bool Is_watch_change();

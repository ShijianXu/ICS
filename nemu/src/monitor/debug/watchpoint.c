#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

uint32_t expr(char *e,bool*success);

static WP wp_list[NR_WP];
static WP *head, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
		wp_list[i].value = 0;
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */


/***********FROM HERE,my code**************/
WP* new_wp(char *arg)
{

	uint32_t result;
	bool ok=true;

	if(free_==NULL)
		assert(0);

	WP *q=head;
	for(;q!=NULL;q=q->next)
	{
		if(strcmp(q->exp,arg)==0)
		{
			printf("The expression has been watched!\n");
			return head;
		}
	}


	WP *j;
	j=free_;
	free_=j->next;
	j->next=NULL;
	
	result=expr(arg,&ok);
	if(ok)
	{
		strcpy(j->exp,arg);
		j->value=result;
		if(head==NULL)
			head=j;
		else
		{
			j->next=head;
			head=j;
		}
	}
	else
		printf("Wrong expression!\n");
	return head;
}



void free_wp(WP *wp)
{
	WP *p=head,*pre=head;
	for(;p->NO!=wp->NO;p=p->next);
	if(p==head)
		head=p->next;
	else
	{
		for(;pre->next!=p;pre=pre->next);
		pre->next=p->next;
	}

	p->next=NULL;
	p->next=free_;
	free_=p;						
}

void delete_watch(int i)
{
	WP *q=head;
	for(;q!=NULL;q=q->next)
	{
		if(q->NO==i)
		{
			free_wp(q);
			return;
		}
	}
	if(q==NULL)
		printf("NO SUCH WATCH POINT\n");
}

void print_list()
{
	WP *p=head;
	int count=0;
	if(head==NULL)
		return;
	else
	{
		for(;p!=NULL;p=p->next)
			count++;
		printf("num of watchpoints > %d\n",count);
		printf("what the points are >\n");
		for(p=head;p!=NULL;p=p->next)
		{	
			printf("%d\t%s\t%02x\n",p->NO,p->exp,p->value);
			count++;
		}
				
	}
}

bool Is_watch_change()
{
	uint32_t result;	
	bool ok=true;

	bool change=false;

	WP *q=head;

	if(head!=NULL)
	{
		for(;q!=NULL;q=q->next)
		{
			result=expr(q->exp,&ok);
			if(result!=q->value)
			{	
				printf(">> %s\n",q->exp);
				printf("old value > %02x\n",q->value);
				printf("new value > %02x\n",result);
				q->value=result;
	//			return true;
				change=true;
			}
//			else
//				return false;
		}
	}
//	else 
//		return false;

return change;
}

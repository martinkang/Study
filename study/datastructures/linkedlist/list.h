#ifndef __LIST_H__
#define __LIST_H__

typedef struct node
{
	int data;
	struct node *next;
} node_t;

typedef struct list
{
	node_t *cur;
	node_t *prev;
	node_t *head;
	int (*comp)(int d1, int d2 );
	int numList;
} list_t;

#endif

void initlist( list_t *plist );

int lsearch( list_t *plist, int data );

int lnext( list_t *plist );

void lfirst( list_t *plist );

int linsert( list_t *plist, int data );

int ldelete( list_t *plist, int data );

void lprint( list_t *plist );

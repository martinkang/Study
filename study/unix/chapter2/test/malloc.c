#include <stdlib.h>
#include <stdio.h>

typedef struct node
{
	char *sPtr;
	int num;
}

typedef struct list
{
	node *node;
	list *next;
} list_t;

static list_t *gList;

void newList()
{
}

int main()
{
	gList = (list_t*)malloc( sizeof(list_t) );
	
}


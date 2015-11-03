#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef struct node
{
	void *data;
	struct node *mNext;
} node_t;

typedef struct queue
{
	node_t *mHead;
	node_t *mTail;
	int numQue;
} que_t;

#endif

void initQue( que_t *que );

int queue( que_t *que, void *aData, int size );

int dequeue( que_t *que, void *aData, int size );

int getQueNum( que_t *que );

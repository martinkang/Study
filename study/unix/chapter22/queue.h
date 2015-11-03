#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef struct queue_node
{
	int fd;
	struct queue_node *mPrev;
} q_node;

typedef struct queue
{
	int mNum_que;
	q_node *mHead;
	q_node *mTail;
} que;

#endif

void q_init( que *aQue );

int q_pop( que *aQue, int *aFd );

int q_push( que *aQue, int *aFd );

void q_finalize( que *aQue );

#ifndef __QUEUE_H__
#define __QUEUE_H__

typedef struct queue_node
{
	char *mStr;
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

int q_pop( que *aQue, char **aData );

int q_push( que *aQue, char *aData );

void q_finalize( que *aQue );

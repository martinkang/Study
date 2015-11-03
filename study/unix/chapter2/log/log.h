#ifndef __LOG_H__
#define __LOG_H__

#include <time.h>

typedef struct data_struct 
{
	time_t mTime;
	char *mStr;
} data_t;

typedef struct list_struct                                         
{
    data_t *mItem;
    struct list_struct *mNext;
} log_t;

#endif

#define IDE_LOG_NUM 1
#define IDE_DE 0
#define IDE_SM 1
#define IDE_CM 2

void ideLog( int aLog, char *aStr );
void clearLog( int aLog ); // clear log
void printLog( int aLog ); // print all log
void viewLastLog( int aLog ); // print tail Log
int saveLog( int aLog );

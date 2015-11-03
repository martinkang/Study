#ifndef __LOG_H__
#define __LOG_H__

#include <time.h>

typedef struct data_struct 
{
	time_t mtime;
	char *mStr;
} data_t;

typedef struct list_struct                                         
{
    data_t *mItem;
    struct list_struct *mNext;
} log_t;

#endif

#define IDE_LOG_NUM 1
#define IDE_DEFAULT 0

void log( int aLog, char *aStr );
static data_t *addLog( char *aStr );
static int addMsg( int aLog, data_t *data ); // add log
static void initAllList();
static void initList( int aLog );
static void clearLog( int aLog ); // clear log
void printLog( int aLog ); // print all log
void getLog( int aLog ); // get curPtr log
//int saveLog( int aLog ); // save disk & clear log

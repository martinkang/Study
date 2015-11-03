#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "log.h"

static gNumLog = 0;

typedef struct list
{
	log_t *headPtr;
	log_t *tailPtr;
	int mNumOfLogs;
} list_t;

static list_t **gList;

static pthread_mutex_t gLogMutex = PTHREAD_MUTEX_INITIALIZER;


static char *gLogName[] = { "IDE_DEFAULT", "IDE_STORAGE_MANAGER",
					"IDE_CONNECT_MANAGER", "IDE_SIGNAL" };

static char *gPath[] = { "default.log", "sm.log", "cm.log", "signal.log" };

static void initList( int aLog )
{
	gList[aLog] = (list_t *)malloc( sizeof(list_t) );
	
	gList[aLog]->headPtr = (log_t*)malloc( sizeof(log_t) );
	gList[aLog]->tailPtr = (log_t*)malloc( sizeof(log_t) );

	gList[aLog]->headPtr = NULL;
	gList[aLog]->tailPtr = NULL;
	gList[aLog]->mNumOfLogs = 0;
}

static void initAllList()
{
	int i = 0;
	
	gList = (list_t **)malloc( sizeof(list_t *) * IDE_LOG_NUM );

	for ( i = 0; i < IDE_LOG_NUM; i++ )
	{
		initList( i );
	}
}

static data_t *newData( const char *aStr )
{
	int sLen = 0;
	data_t *sNewData = (data_t*)malloc( sizeof(data_t) );

	sLen = strlen( aStr );

	if ( aStr[sLen - 1] != '\0' )
	{
		sLen += 1;
	}

	sNewData->mStr = (char*)malloc( sizeof(char) * sLen );
	strcpy( sNewData->mStr, aStr );
	sNewData->mStr[sLen - 1] = '\0';
	time( &(sNewData->mTime) );

	return sNewData;
}

static void addLog( int aLog, data_t *aNewData )
{
	log_t *sNewLog = (log_t*)malloc( sizeof(log_t) );
	
	sNewLog->mItem = aNewData;
	sNewLog->mNext = NULL;

	if ( gList[aLog]->headPtr == NULL )
	{
		gList[aLog]->headPtr = sNewLog;
		gList[aLog]->headPtr->mNext = gList[aLog]->tailPtr;
		gList[aLog]->tailPtr = sNewLog;
	}
	else
	{
		gList[aLog]->tailPtr->mNext = sNewLog;
		gList[aLog]->tailPtr = sNewLog;
	}
	
	(gList[aLog]->mNumOfLogs)+=1;
}

static void autoClearLog( int aLog )
{
	log_t *sPtr = NULL;
	log_t *sDel = NULL;

	if ( gList[aLog]->headPtr != NULL )
	{
		sPtr = gList[aLog]->headPtr;

		while ( sPtr != NULL )
		{
			sDel = sPtr;
			sPtr = sPtr->mNext;

			free( sDel->mItem->mStr );
			free( sDel->mItem );

			sDel->mItem->mStr = NULL;
			sDel->mItem = NULL;
		}

		if ( gList[aLog]->headPtr != gList[aLog]->tailPtr )
		{
			if ( gList[aLog]->tailPtr != NULL )
			{
				free( gList[aLog]->tailPtr );
				gList[aLog]->tailPtr = NULL;
			}
		}

		free( gList[aLog]->headPtr );
		gList[aLog]->headPtr = NULL;

	
	}
	else
	{
		printf( "\n%s Log Clear is Failure\nLog is NULL\n", gLogName[aLog] );
	}	
}

static int autoSaveLog( int aLog )
{
	FILE *sFile = NULL;
	log_t *sPtr;

	if ( gList[aLog]->headPtr != NULL )
	{
		sFile = fopen( gPath[aLog], "a" );

		sPtr = gList[aLog]->headPtr;
		while ( sPtr != NULL )
		{
			fprintf( sFile, "%s\n%s\n", sPtr->mItem->mStr, ctime( &(sPtr->mItem->mTime ) ) );
			sPtr = sPtr->mNext;
		}

		fclose( sFile );

		autoClearLog( aLog );

		return 0;
	}
	else
	{
		printf( "%s Log is Empty. Save Log Fail\n", gLogName[aLog] );
	
		return -1;
	}
}

void ideLog( int aLog, char *aStr )
{
	data_t *sNewData = NULL;

	pthread_mutex_lock ( &gLogMutex );

	if ( gList == NULL )
	{
		initAllList();
	}
	else
	{
		/* do nothing */
	}
	if ( gList[aLog]->headPtr == NULL )
		initList(aLog);

	sNewData = newData( aStr );
	addLog( aLog, sNewData );

	gNumLog++;

	if ( gNumLog >= 5 )
		autoSaveLog( aLog );

	pthread_mutex_unlock ( &gLogMutex );
}

void printLog( int aLog )
{
	log_t *sPtr = NULL;
	
	printf("==============================\n"
		   "           Print Log          \n"
		   "==============================\n" );

	pthread_mutex_lock ( &gLogMutex );

	if ( gList[aLog]->headPtr != NULL )
	{
		sPtr = gList[aLog]->headPtr;
		while ( sPtr != NULL )
		{
			printf( "Last Log : %s\nTime : %s\n", sPtr->mItem->mStr, ctime( &(sPtr->mItem->mTime) ) ); 

			sPtr = sPtr->mNext;
		}
	}
	else
	{
		printf( "Log[%s] is NULL\n", gLogName[aLog] );
	}
	
	pthread_mutex_unlock( &gLogMutex );
}

void viewLastLog( int aLog )
{
	data_t *sPtr  = NULL;

	printf("==============================\n"
		   "          Last Log            \n"
		   "==============================\n" );
	pthread_mutex_lock ( &gLogMutex );

	if ( gList[aLog]->tailPtr != NULL )
	{
		sPtr = gList[aLog]->tailPtr->mItem;
		printf( "Last Log : %s\nTime : %s\n", sPtr->mStr, ctime( &(sPtr->mTime) ) ); 
	}
	else
	{
		printf( "Log[%s] is NULL\n", gLogName[aLog] );
	}
	
	pthread_mutex_unlock ( &gLogMutex );
}

void clearLog( int aLog )
{
	log_t *sPtr = NULL;
	log_t *sDel = NULL;

	pthread_mutex_lock ( &gLogMutex );
	if ( gList[aLog]->headPtr != NULL )
	{
		sPtr = gList[aLog]->headPtr;

		while ( sPtr != NULL )
		{
			sDel = sPtr;
			sPtr = sPtr->mNext;

			free( sDel->mItem->mStr );
			free( sDel->mItem );

			sDel->mItem->mStr = NULL;
			sDel->mItem = NULL;
		}

		if ( gList[aLog]->headPtr != gList[aLog]->tailPtr )
		{
			if ( gList[aLog]->tailPtr != NULL )
			{
				free( gList[aLog]->tailPtr );
				gList[aLog]->tailPtr = NULL;
			}
		}

		free( gList[aLog]->headPtr );
		gList[aLog]->headPtr = NULL;

	
	}
	else
	{
		printf( "\n%s Log Clear is Failure\nLog is NULL\n", gLogName[aLog] );
	}	
	
	pthread_mutex_unlock ( &gLogMutex );
}

int saveLog( int aLog )
{
	FILE *sFile = NULL;
	log_t *sPtr;

	pthread_mutex_lock ( &gLogMutex );

	if ( gList[aLog]->headPtr != NULL )
	{
		sFile = fopen( gPath[aLog], "a" );

		sPtr = gList[aLog]->headPtr;
		while ( sPtr != NULL )
		{
			fprintf( sFile, "%s\n%s\n", sPtr->mItem->mStr, ctime( &(sPtr->mItem->mTime ) ) );
			sPtr = sPtr->mNext;
		}

		fclose( sFile );

		clearLog( aLog );

		pthread_mutex_unlock ( &gLogMutex );

		return 0;
	}
	else
	{
		printf( "%s Log is Empty. Save Log Fail\n", gLogName[aLog] );

		pthread_mutex_unlock ( &gLogMutex );

		return -1;
	}
}


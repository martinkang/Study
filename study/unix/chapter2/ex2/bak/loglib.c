#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "../../ide.h"

typedef struct data_list
{
	log_t *headPtr;
	log_t *tailPtr;
	int mNumOfPtr;
} list_t;

static list_t **gList;
static char *sPath;

static int addMsg( int aLog, data_t *data )
{
	log_t *newLog = (log_t*)malloc( sizeof(log_t) );
	
	newLog->mItem = data;
	newLog->mNext = NULL;

	if ( gList[aLog]->headPtr != NULL )
	{
		gList[aLog]->tailPtr->mNext = newLog;
		gList[aLog]->tailPtr = newLog;
	}
	else
	{
		newLog->mNext = gList[aLog]->tailPtr;
		gList[aLog]->headPtr = newLog;
		gList[aLog]->tailPtr = newLog;
	
	}

	( gList[aLog]->mNumOfPtr ) += 1;

	return 0;
}

static data_t *addLog( char *aStr )
{
	int len = 0;
	data_t newLog;

	len = strlen( aStr );

	if ( aStr[len-1] != '\0' )
		len += 1;

	newLog.mStr = (char*)malloc( sizeof(char) * len );

	strcpy( newLog.mStr, aStr );
	newLog.mStr[len -1] = '\0';

	time( &( newLog.mtime ) );

	return &newLog;
}

void log( int aLog, char *aStr )
{
	int len = 0;
	data_t *newLog = NULL;

	if ( gList == NULL )
	{
		initAllList();
	}

	newLog = addLog( aStr );
	addMsg( aLog, newLog );

	if ( gList[aLog]->mNumOfPtr >= 10 )
	{
		//saveLog( aLog );
		//clearLog( aLog );
	}
}

static void initList( int aLog )
{
	gList[aLog] = (list_t *)malloc( sizeof(list_t) );
	gList[aLog]->headPtr = (log_t*)malloc( sizeof(log_t) );
	gList[aLog]->tailPtr = (log_t*)malloc( sizeof(log_t) );
	gList[aLog]->mNumOfPtr = 0;

	gList[aLog]->headPtr = NULL;
	gList[aLog]->tailPtr = NULL;
}

static void initAllList()
{
	int i = 0;

	gList = (list_t **)malloc( sizeof(list_t *) * IDE_LOG_NUM );

	for ( i = 0; i < IDE_LOG_NUM; i++ )
	{
		gList[i] = (list_t *)malloc( sizeof(list_t) );
		gList[i]->headPtr = (log_t*)malloc( sizeof(log_t) );
		gList[i]->tailPtr = (log_t*)malloc( sizeof(log_t) );
		gList[i]->mNumOfPtr = 0;

		gList[i]->headPtr = NULL;
		gList[i]->tailPtr = NULL;
	}
}

static void clearLog( int aLog )
{
	log_t *sDel = NULL;
	log_t *sPos = NULL;
	int sRC = 0;

	if ( gList[aLog]->headPtr == NULL )
	{
		printf( "Log is Already clear\n" );
	}
	else
	{
		sDel = gList[aLog]->headPtr;

		while ( sDel != NULL )
		{
			sPos = sDel;
			sDel = sDel->mNext;
			
			(gList[aLog]->mNumOfPtr) -= 1;
			free( sPos );
		}

		initList( aLog );
	}
}

void printLog( int aLog )
{
	int i = 0;
	log_t *sPos = NULL;

	printf( "\n================\nPrint ALL LOG[%d]\n================\n" );

	if ( gList[aLog]->headPtr != NULL )
	{
		sPos = gList[aLog]->headPtr;
		
		while( sPos != NULL )
		{
			printf( "\n%s\n%s\n", sPos->mItem->mStr, ctime( sPos->mItem->mtime ) );

			sPos = sPos->mNext;
		}
	}
	else
	{
		printf( "Log is NULL\n" );
	}
	
	printf( "================\n" );
}

void getLog( int aLog )
{
	data_t sPos;
	int len = 0;
	int sRC = 0;

	if ( gList[aLog]->headPtr != NULL )
	{
		len = strlen( gList[aLog]->tailPtr->mItem->mStr );

		sPos.mStr = (char*)malloc( sizeof(char) * len );
		strcpy( sPos.mStr, gList[aLog]->tailPtr->mItem->mStr ); 
		sPos.mtime = gList[aLog]->tailPtr->mItem->mtime;

		printf( "getLog : %s\n%s\n", sPos.mStr, ctime( &( sPos.mtime ) ) );
	}
	else
	{
		printf( "Log List is NULL\n" );
	}
}

/*
int saveLog( int aLog )
{
	const char *sPath = filename;
	log_t *sPos;
	FILE *sFile = NULL;
	char *sPtr = NULL;
	int len1 = 0;
	int len2 = 0;
	int sRC = 0;

	sPos = gList[aLog]->headPtr;

	sFile = fopen( sPath, "a" );

	while ( sPos != NULL )
	{
		len1 = strlen( sPos->mItem->mStr );
		sPos = sPos->mNext;	
	}

	clearLog( aLog );

	fclose(sFile);

}
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "../ide.h"
#include "ntpvm.h"
#include "dispatcher.h"

#define BUFSIZE 1024

static task_t *gTask[MAX_TASKS];

static void initTask( void )
{
	int i = 0;

	for ( i = 0; i < MAX_TASKS; i++ )
	{
		gTask[i] = NULL;
	}
}

static void finalTask( void )
{
	int i = 0;

	for ( i = 0; i < MAX_TASKS; i++ )
	{
		if ( gTask[i] != NULL )
		{
			free( gTask[i] );
			gTask[i] = NULL;
		}
	}
}

static int makeInOutThread()
{

}

int main( void )
{
	int sRC = 0;
	int step = 0;
	int loop = 0;
	int size = 0;

	char buf[BUFSIZE] = {0, };

	int fd[2]; // for get, put thread
	
	pthread_t getThread;
	pthread_t putThread;

	taskpacket_t *newPacket;
	int psize = 0;

	IDE_TEST( pipe( fd ) );
	IDE_TEST( dup2( fd[0], STDIN_FILENO ) );		
	IDE_TEST( dup2( fd[1], STDIN_FILENO ) );
	IDE_TEST( close( fd[0] ) );
	IDE_TEST( close( fd[1] ) );

	initTask();
	
	psize = sizeof( taskpacket_t );

	while ( loop == 0 )
	{
		newPacket = CreatePacket( (void*)fd );
		step = 1;
		size = read( fd[0], newpacket, sizeof( newpacket ) );
		if ( size != psize )
		{
			fprintf( stderr, "getpacket error" );
		}
		else
		{
			sRC = putpacket( fdout, newPacket, buf );
			if ( sRC == -1 )
			{
				fprintf( stderr, "getpacket error\n" );
				continue;
			}
		}
		else
		{
			fprintf( stderr, "getpacket error\n" );
			continue;
		}
		fprintf( stderr, "Ready for next packet\n" );
	}

	finalTask();

	return 0;

	EXCEPTION_END

	switch ( step )
	{
		case :
			break;
		default :
			break;
	}

	perror( "error" );

	return -1;
}

taskpacket_t *CreatePacket( void )
{
	taskpacket_t *newPack = NULL;

	newPack = (taskpacket_t*)malloc( sizeof(taskpacket_t) );
	
	return newPack;
}

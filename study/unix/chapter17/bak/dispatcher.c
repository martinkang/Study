#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ntpvm.h"
#include "proc.h"
#include "dispatcher.h"

static task_t *gTask[MAX_TASKS];

task_t *newTask( taskpacket_t *Header  );
taskpacket_t *newHeader( int compid, int taskid, packet_t type, int tdatalen );

static void initTask( void )
{
	int i = 0;

	for ( i = 0; i < MAX_TASKS; i++ )
		gTask[i] = NULL;
}

static int findTask( int compid, int taskid )
{
	for ( i = 0; i < MAX_TASKS; i++ )
	{
		if ( gTask[i] != NULL )
		{
			if ( ( gTask[i]->compid == compid ) &&
					( gTask[i]->taskid == taskid ) )
			{
				return i;
			}
		}
	}

	return -1;
}

static int findEmptyTask( )
{
	for ( i = 0; i < MAX_TASKS; i++ )
	{
		if ( ( gTask[i] == NULL) )
		{
			return i;
		}
	}

	return -1;
}

int main( void )
{
	/* etc */
	int sRC = 0;
	int size = 0;

	/* packet */
	int compid;
	int taskid;
	int tdatalen;
	int tin, tout;
	int idx = 0;
	packet_t type;

	taskpacket_t *newheader = NULL;
	task_t *newtask = NULL;

	/* fork */
	int pid = 0;

	/* pipe */
	int fd[2];

	tin = STDIN_FILENO;
	tout = STDOUT_FILENO;

	initTask();

	if ( sRC == -1 )
		return -1;

	while( sRC != -1 )
	{
		sRC = pipe( fd );

		sRC = getpacket( tin, &compid, &taskid, &type, &tdatalen, buf );
		if ( sRC != -1 )
		{
			newheader = newHeader( compid, taskid, type, tdatalen );
			idx = findTask( newheader->compid, newheader->taskid );
			switch ( type )
			{
				case 0:
					if ( idx != -1 )
					{
						idx = findEmptyTask();
						if ( idx != -1 )
						{
							newtask = newTask( newHeader );
							gTask[idx] = newtask;
						}
						else
						{
							fprintf( stderr, "already full task set\n" );
						}
					}
					else
					{
						fprintf( stderr, "comp id : %d task id : %d is already hear\n", compid, taskid );
						continue;
					}

				case 1:
				case 2:
					fprintf( stderr, "net yet\n" );
					continue;
				case 3:
				case 4:
					fprintf( stderr, "terminate\n" );
					break;
				case 5:
					fprintf( stderr, "not yet\n" );
					continue;
				default :
					break;
			}
			sRC = putpacket();
			if ( sRC == -1 )
				return -1;
		}
		else
		{
			/* do nothing */
		}

		fprintf( stderr, "Ready for next packet\n\n" );
	}

	close ( fd[0] );
}

taskpacket_t *newHeader( int compid, int taskid, packet_t type, int tdatalen )
{
	taskpacket_t *newHeader;

	newHeader = (taskpacket_t*)malloc( sizeof( newHeader ) );

	newHeader->compid = compid;
	newHeader->taskid = taskid;
	newHeader->type = type;
	newHeader->length = tdatalen;

	return newHeader;
}

task_t *newTask( taskpacket_t *Header  )
{
	task_t *newtask;

	newtask = (task_t)malloc( sizeof( task_t ) );

	newtask->compid = Header->compid;
	newtask->taskid = Header->taskid;
	newtask->writefd = 0;
	newtask->readfd = 0;
	newtask->recvbytes = 0;
	newtask->recvpacksets = 0;
	newtask->sentbytes = 0;
	newtask->sentpacksets = 0;
	newtask->taskpid = -1;
	newtask->tasktid = -1;
	newtask->barrier = -1;
	newtask->endinput = 0;
	
	return newtask;
}


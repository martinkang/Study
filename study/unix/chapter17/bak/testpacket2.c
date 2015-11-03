#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ntpvm.h"
#include "proc.h"
#include "dispatcher.h"


static typedef struct taskset
{
	int taskid;
	struct taskset *mNext;
} taskset_t;

static typedef struct header
{
	int compid;
	taskset *mSet[MAX_TASKS];
} header_t;

static header_t Header;

static char *typename[] = { "newtask", "data", "broadcast",
                            "done", "terminate", "barrier" };

static int putpacket( int fdout, int compid, int taskid, packet_t type, int tdatalen, unsigned char * buf )
{
	taskpacket_t header;
	int headlen = 0;
	int size = 0;
	if ( tdatalen < 0 || tdatalen > MAX_PACK_SIZE )
		return -1;

	headlen = sizeof( header );
	header.compid = compid;
	header.taskid = taskid;
	header.type = type;
	header.length = tdatalen;

	size = write( fdout, &header, headlen );
	if ( size != headlen )
		return -1;

	size = write( fdout, buf, tdatalen );
	if ( size != tdatalen )
		return -1;
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

task_t *newTask( taskpacket_t *Header, char *buf)
{

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
	packet_t type;

	taskpacket_t *newheader = NULL;
	task_t *newtask = NULL;

	/* fork */
	int pid = 0;

	/* pipe */
	int fd[2];

	tin = STDIN_FILENO;
	tout = STDOUT_FILENO;

	if ( sRC == -1 )
		return -1;

	while( sRC != -1 )
	{
		sRC = pipe( fd );

		sRC = getpacket( tin, &compid, &taskid, &type, &tdatalen, buf );
		if ( sRC != -1 )
		{
			newheader = newHeader( compid, taskid, type, tdatalen );

			switch ( type )
			{
				case 0:
					if ( )
					{
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
		}
		else
		{
			/* do nothing */
		}

		fprintf( stderr, "Ready for next packet\n\n" );
	}

	close ( fd[0] );
}

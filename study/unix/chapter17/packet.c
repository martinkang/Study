#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ide.h"
#include "restart.h"
#include "dispatcher.h"
#include "ntpvm.h"

#define MAX_LINE_SIZE 80

static char *typename[] = { "newtask", "data", "broadcast",
                            "done", "terminate", "barrier" };

void *getpacket( void *arg )
{
    int sRC = 0;
    int i = 0;
	int loop = 0;
    int size = 0;

	int fdin = STDIN_FILENO;
	int fdout = ((int*)fd)[1];
	taskpacket_t newPack;

    int linelen = 0;
    int len = 0;
	char buf[MAX_LINE_SIZE] = {0, };
    char *bufp = NULL;

	while( loop == 0 )
	{
		memset( buf, 0, MAX_LINE_SIZE );
		memset( &newPack, 0, sizelf( newPack ) );

		fprintf( stderr, "\compid : " );
		scanf( "%d", &( newPack.compid ) );

		fprintf( stderr, "taskid : " );
		scanf( "%d", &( newPack.taskid ) );

		fprintf( stderr, "TYPES\n" );
		for ( i = 0; i < NUMTYPES; i++ )
			fprintf( stderr, "[%d] : %s\n", i, typename[i] );

		fprintf( stderr, "type : " );
		scanf( "%d", &( newPack.type ) );

		if ( newPack.type == 2 || newPack.type == 4 || newPack.type == 5 )
		{
			fprintf( stderr, "not ready yet\n\n" );
			continue;
		}
		else
		{
			bufp = buf;
			fprintf( stderr, "input command line ( %s is to end ) : ", TERMINATE_STRING );
			while ( ( linelen = readline( fdin, bufp, MAX_LINE_SIZE ) ) != -1 )
			{
				if ( linelen == 0 )
				{
					sRC = 1;
					break;
				}

				if ( strncmp( TERMINATE_STRING, bufp, strlen( TERMINATE_STRING ) ) == 0 )
				{
					buf[len] = 0;
					break;
				}

				if ( len >= MAX_PACK_SIZE )
				{
					fprintf( stderr, "maximum packet size exceeded\n" );
					sRC = 1;
					break;
				}

				if ( bufp[linelen - 1] == '\n' )
					bufp[linelen - 1] = '\0';

				len = len + linelen;
				fprintf( stderr, "Received %d bytes, Total %d bytes\nRecived string : %s\n", linelen, len, bufp );
				bufp = bufp + linelen;
				fprintf( stderr, "input command line ( %s is to end ) : ", TERMINATE_STRING );
			}

			if ( sRC == 0 )
			{
				newPack.length = len;

				size = write( fdout, &newPack, sizeof( newPack ) );
				if ( size != sizeof( newPack ) )
				{
					fprintf( stderr, "getpacket to pipe error\n" );
					continue;
				}

				size = write( fdout, buf, len );
				if ( size != len )
				{
					fprintf( stderr, "getpacket to pipe error\n" );
					continue;
				}
			}
		}
	}

	pthread_exit();
}

int putpacket( int fdout, taskpacket_t *pack, unsigned char *buf )
{
	int sRC = 0;
	int size = 0;

	int len = 0;
	char msg[200] = {0, };

	snprintf( msg, 100, "compid : %d\ntaskid : %d\ntype : %s\nlength : %d\ndata : ",
			pack->compid, pack->taskid, typename[pack->type], pack->length );

	len = strlen( msg );
	size = write( fdout, msg, len );
	if ( size != len )
		return -1;

	size = write( fdout, buf, pack->length );
	if ( size != pack->length )
		return -1;

	size = write( fdout, "\n", 1 );

	return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "restart.h"
#include "ntpvm.h"

#define MAX_LINE_SIZE 80

static char *typename[] = { "newtask", "data", "broadcast",
                            "done", "terminate", "barrier" };

int main( void )
{
	int sRC = 0;
	int i = 0;
	int loop = 0;
	int size = 0;

	int linelen = 0;
	int len = 0;
	char buf[MAX_LINE_SIZE] = {0, };
	char *bufp = NULL;
	char endstr[1] = {0, };

	int psize;
	packet_t tasktype;
	taskpacket_t pack;
	task_t task;

	psize = sizeof( pack );

	while ( loop == 0 )
	{
		fprintf( stderr, "compid : " );
		scanf( "%d", &pack.compid );

		fprintf( stderr, "taskid : " );
		scanf( "%d", &pack.taskid );


		for ( i = 0; i < NUMTYPES; i++ )
		{
			fprintf( stderr, "[%d] %s\n", i + 1, typename[i] );
		}

		fprintf( stderr, "type : " );
		scanf( "%d", &tasktype );
		pack.type = tasktype;

		bufp = buf;
		fprintf( stderr, "input command line ( %s is to end ) : ", TERMINATE_STRING );
		while ( ( linelen = readline( STDIN_FILENO, bufp, MAX_LINE_SIZE ) ) != -1 )
		{
			if ( linelen == 0 )
				break;

			if ( strncmp( TERMINATE_STRING, bufp, strlen( TERMINATE_STRING ) ) == 0 )
				break;

			if ( len >= MAX_PACK_SIZE )
			{
				fprintf( stderr, "maximum packet size exceeded\n" );
				return -1;
			}

			len = len + linelen;
			fprintf( stderr, "Received %d bytes, Total %d bytes\nRecived string : %s\n", linelen, len, bufp );
			bufp = bufp + linelen;
		}

		pack.length = len;

		size = write( STDOUT_FILENO, &pack, sizeof( pack ) );
		if ( size != sizeof( pack ) )
			return -1;

		size = write( STDOUT_FILENO, buf, sizeof( buf ) );
		if ( size != sizeof( buf ) )
			return -1;

		fprintf( stderr, "next(n) or end(e)) : " );
		read( STDIN_FILENO, endstr, 1 );
		size = write( STDOUT_FILENO, endstr, 1 );
		if ( strncmp( endstr, "e", 1 ) == 0 )
		{
			loop = 1;
			fprintf( stderr, "*** End ***\n" );
		}
		else
		{
			fprintf( stderr, "Ready for next packet\n" );
		}
	}

	return 0;
}

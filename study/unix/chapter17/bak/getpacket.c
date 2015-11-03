#include <stdio.h>
#include <string.h>
#include "restart.h"
#include "ntpvm.h"

#define MAX_LINE_SIZE 80

static char *typename[] = { "newtask", "data", "broadcast",
                            "done", "terminate", "barrier" };

int getpacket( int fdin, int * compid, int * taskid, packet_t * type, int * tdatalen, unsigned char * buf )
{
    int sRC = 0;
    int i = 0;
    int size = 0;

    int linelen = 0;
    int len = 0;
    char *bufp = NULL;

    fprintf( stderr, "compid : " );
    scanf( "%d", compid );

    fprintf( stderr, "taskid : " );
    scanf( "%d", taskid );

	fprintf( stderr, "TYPES\n" );
	for ( i = 0; i < NUMTYPES; i++ )
		fprintf( stderr, "[%d] : %s\n", i, typename );
    
	fprintf( stderr, "type : " );
    scanf( "%d", type );

	if ( type == 2 || type == 4 || type == 5 )
	{
		*tdatalen = 0;
		buf = NULL;
		return 0;
	}

	bufp = buf;
    fprintf( stderr, "input command line ( %s is to end ) : ", TERMINATE_STRING );
    while ( ( linelen = readline( STDIN_FILENO, bufp, MAX_LINE_SIZE ) ) != -1 )
    {
        if ( linelen == 0 )
            break;

        if ( strncmp( TERMINATE_STRING, bufp, strlen( TERMINATE_STRING ) ) == 0 )
        {
            buf[len] = 0;
            break;
        }

        if ( len >= MAX_PACK_SIZE )
        {
            fprintf( stderr, "maximum packet size exceeded\n" );
            return -1;
        }

        if ( bufp[linelen - 1] == '\n' )
            bufp[linelen - 1] = '\0';

        len = len + linelen;
        fprintf( stderr, "Received %d bytes, Total %d bytes\nRecived string : %s\n", linelen, len, bufp );
        bufp = bufp + linelen;
    }

    *tdatalen = len;

    fprintf( stderr, "\n" );
    return 0;
}


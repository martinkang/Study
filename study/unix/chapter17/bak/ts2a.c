#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ntpvm.h"

#define MAX_LINE_SIZE 80

static char *typename[] = { "newtask", "data", "broadcast",
                            "done", "terminate", "barrier" };

int main( void )
{
	int sRC = 0;
	int i = 0;
	int loop = 0;

	int psize;
	packet_t type;
	taskpacket_t pack;
	task_t task;
	char buf[MAX_LINE_SIZE] = {0, };
	char endstr[1] = {0, };

	int size = 0;

	psize = sizeof( pack );

	while ( loop == 0 )
	{
		size = read( STDIN_FILENO, &pack, psize );
		if ( size != psize )
			return -1;

		size = read( STDIN_FILENO, buf, MAX_LINE_SIZE );
		if ( size != psize )
			return -1;

		fprintf( stdout, "compid : %d\ntaskid : %d\ntype : %s\nlen : %d\nstr : %s\n", 
				pack.compid, pack.taskid, typename[pack.type], pack.length, buf );

		if ( pack.length < 0 | pack.length > MAX_PACK_SIZE )
		{
			fprintf( stderr, "task data is to long or short\n" );
			return -1;
		}

		size = write( STDOUT_FILENO, &pack, psize );
		if ( size != psize )
			return -1;

		size = write( STDOUT_FILENO, buf, MAX_LINE_SIZE );
		if ( size != psize )
			return -1;

		size = read( STDIN_FILENO, endstr, 1 );
		if ( strncmp( endstr, "e", 1 ) == 0 )
		{
			fprintf( stdout, " *** end ***\n" ); 
			loop = 1;
		}
		else
		{
			fprintf( stdout, "Ready for next packet\n" );
		}

	}
	return 0;
}

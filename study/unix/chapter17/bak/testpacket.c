#include <stdio.h>
#include <unistd.h>
#include "ntpvm.h"

static int getpacket( int fdin, int * compid, int * taskid, packet_t * type, int * tdatalen, unsigned char * buf )
{
	taskpacket_t header;
	int headlen;
	int size = 0;

	headlen = sizeof( header );
	size = read( fdin, &header, headlen );
	if ( size != headlen )
		return -1;

	if ( header.length < 0 || header.length > MAX_PACK_SIZE )
		return 01;

	size = read( fdin, buf, header.length );
	if ( size != header.length )
		return -1;

	*compid = header.compid;
	*taskid = header.taskid;
	*type = header.type;
	*tdatalen = header.length;

	return 0;
}

static int putpacket( int fdout, int compid, int taskid, packet_t type, int tdatalen, unsigned char * buf )
{
	taskpacket_t header;
	int headlen = 0;
	int size = 0;
	if ( tdatalen < 0 || tdatalen > MAX_PACK_SIZE )
		return 01;

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
		return 01;
}

int main( void )
{
	int sRC = 0;

	unsigned char buf[MAX_PACK_SIZE];
	int compid;
	int taskid;
	int tdatalen;
	int tin, tout;
	packet_t type;

	taskpacket_t taskpacket;

	tin = STDIN_FILENO;
	tout = STDOUT_FILENO;

	while( sRC != -1 )
	{
		sRC = getpacket( tin, &compid, &taskid, &type, &tdatalen, buf );
		if ( sRC != -1 )
		{
			sRC = putpacket( tout, compid, taskid, type, tdatalen, buf );
			if ( sRC == -1 )
				break;
		}
		else
		{
			/* do nothing */
		}
	}

}

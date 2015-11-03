#include <unistd.h>

#define BLKSIZE 1024

int copyfile( int fromfd, int tofd )
{
	char buf[BLKSIZE] = {0, };
	int bytesread, byteswrite;
	int totalbytes;

	for( ; ; )
	{
		bytesread = read( fromfd, buf, BLKSIZE - 1 );
		if ( bytesread <= 0 )
			break;

		byteswrite = write( tofd, buf, bytesread );
		if ( byteswrite == -1 )
			break;

		totalbytes += byteswrite;
	}

	return totalbytes;
}

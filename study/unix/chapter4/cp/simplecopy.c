#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define BLKSIZE 1024

int copyfile( int fromfd, int tofd )
{
	char *bp;
	char buf[BLKSIZE];
	int bread, bwrite = 0;
	int tbyte = 0;

	for( ; ; )
	{
		while( ( (bread = read(fromfd, buf, BLKSIZE)) == -1 ) &&
				( errno == EINTR ) );
		if ( bread <= 0 )
		{
			break;
		}

		bp = buf;
		
		while ( bread > 0 )
		{
			while ( ( ( bwrite = write(tofd, bp, bread) ) == -1 ) &&
					(errno == EINTR) );
			if ( bread <= 0 )
				break;

			tbyte += bwrite;
			bread -=bwrite;
			bp += bwrite;
		}
		if ( bwrite == -1 )
			break;
	}

	return tbyte;
}

int main(void)
{
	int numbytes;

	numbytes = copyfile(STDIN_FILENO, STDOUT_FILENO);

	fprintf( stderr, "Number of bytes copied: %d\n", numbytes );
	
	return 0;
}

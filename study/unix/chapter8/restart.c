#include <errno.h>

int r_write( int fdout, char *buf, int b_size )
{
	int w_size = 0;	

	while ( ( w_size = write( fdout, buf, b_size ) ) == -1 && errno == EINTR );

	return w_size;
}

int r_read( int fdin, char *buf, int b_size )
{
	int r_size = 0;

	while ( ( r_size = read( fdin, buf, b_size ) ) == -1 && errno == EINTR );

	return r_size;
i

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

#define ECHOFLAGS ( ECHO | ECHOE | ECHOK | ECHONL )

int readline( int fd, char *buf, int nbytes );
int setecho( int fd, int onflag );

int pd( char *prompt, char *passbuf, int passmax )
{
	int fd;
	int firsterrno = 0;
	int passlen;
	char termbuf[L_ctermid];

	if ( ctermid( termbuf ) == NULL )
	{
		errno = ENODEV;
		return -1;
	}

	if (( fd = open( termbuf, O_RDWR ) ) == -1 )
		return -1;
	
	if ( setecho(fd, 0 ) ==  -1 )
		firsterrno = errno;
	else if ( write( fd, prompt, strlen( prompt ) ) == -1 )
		firsterrno = errno;
	else if ( ( passlen = readline( fd, passbuf, passmax ) )== 0 )
		firsterrno = EINVAL;
	else if ( passlen == -1 )
		firsterrno = errno;
	else
		passbuf[passlen - 1] = '\0';

	if (( setecho( fd, 1 ) == -1 ) && !firsterrno )
		firsterrno = errno;
	if ( write( fd, "\n", 1 ) == -1  && !firsterrno )
		firsterrno = errno;
	if ( close(fd) == -1 && !firsterrno )
		firsterrno = errno;

	if ( firsterrno )
		errno = firsterrno;

	return firsterrno ? -1 : 0;
}

int main(void)
{
	char termbuf[L_ctermid];
	char msg[1024];

	pd( "prompt : ", msg, 1024 );

	printf( "after pd\nmsg : %s\n", msg );
	return 0;
}

int setecho( int fd, int onflag )
{
	int error;
	struct termios term;

	if ( tcgetattr( fd, &term ) == -1 )
		return -1;

	if ( onflag > 0 )
		term.c_lflag |= ECHOFLAGS;
	else
		term.c_lflag &= ~ECHOFLAGS;

	while ((( error = tcsetattr( fd, TCSAFLUSH, &term ) ) == -1 ) && (errno == EINTR ) );

	return error;
}

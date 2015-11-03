#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>

static void showtimes(void)
{
	double ticks;
	struct tms tinfo;

	if ( ( ticks = (double) sysconf(_SC_CLK_TCK ) ) == -1 )
		perror( "Failed to determine clock ticks per second" );
	else if ( times(&tinfo) == (clock_t) -1 )
		perror( "Failed to get times information" );
	else
	{
		fprintf( stderr, "User time :                        %8.3f seconds\n", tinfo.tms_utime/ticks );
		fprintf( stderr, "System time :                      %8.3f seconds\n", tinfo.tms_stime/ticks );
		fprintf( stderr, "Children`s user time :             %8.3f seconds\n", tinfo.tms_cutime/ticks );
		fprintf( stderr, "Children`s System time :           %8.3f seconds\n", tinfo.tms_cstime/ticks );
	}
}

int main(void)
{
	int i = 0;

	if ( atexit(showtimes))
	{
		fprintf( stderr, "Failed to install showtimes exit handler\n" );
		return 1;
	}

	for ( i = 0; i < 1000000; i++ )
	{
		printf(".");
	}
	printf( "\n\n");

	return 0;
}


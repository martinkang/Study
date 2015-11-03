#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int sRC = 0;
	int sRC2 = 0;

	sRC = system("env | grep LOG" );
	sRC2 = system("env | grep asldkfjdasf" );

	printf( "sRC : %d\n", sRC );
	printf( "sRC2 : %d\n", sRC2 );

	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
	char msg[] = "asdf aaaa dfsa aaa\n";
	char **saveptr = NULL;

	char msg3[] = "aaaa aaaaa bbbb\n";
	char msg2[100] = {0, };
	char *ptr= NULL;

	char *sArgv[3] = { "ls", "-al", NULL };

	strcpy( msg2, msg );

	saveptr = (char**)malloc(sizeof(char) * 200 );

	ptr = strtok_r( msg2, " ", saveptr );
	while ( ptr != NULL )
	{
		printf( "%s\n", ptr );
		ptr = strtok_r( NULL, " ", saveptr );
	}

	strcpy( msg2, msg3 );

	ptr = strtok_r( msg2, " ", saveptr );
	while ( ptr != NULL )
	{
		printf( "%s\n", ptr );
		ptr = strtok_r( NULL, " ", saveptr );
	}

	free( saveptr );
	free( ptr );

	execvp( "ls", sArgv );

	return 0;
}



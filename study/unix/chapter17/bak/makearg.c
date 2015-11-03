#include <string.h>
#include <stdlib.h>

int makearg( const char *origText, char ***argv, const char *delim )
{
	int sRC = 0;
	int i = 0;

	int argc = 0;

	int len = 0;
	char *cpText = NULL;
	char **saveptr = NULL;
	char *ptr = NULL;

	len = strlen( origText );
	saveptr = (char**)malloc( sizeof(char*) * len );
	cpText = (char*)malloc( sizeof(char) * len );

	strcpy( cpText, origText );

	ptr = strtok_r( cpText, delim, saveptr );
	while ( ptr != NULL )
	{
		ptr = strtok_r( NULL, delim, saveptr );
		argc++;
	}

	strcpy( cpText, origText );

	*argv = (char**)malloc( sizeof(char*) * argc + 1 );
	(*argv)[i] = strtok_r( cpText, delim, saveptr );
	while ( (*argv)[i] != NULL )
	{
		i++;
		(*argv)[i] = strtok_r( NULL, delim, saveptr );
	}

	free( saveptr );

	return argc;
}


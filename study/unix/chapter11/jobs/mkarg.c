#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int makeArgc( char *aOrigText, const char *aDelimiter )
{
	int sNumOfResult = 1;

	(void)strtok( aOrigText, aDelimiter );

	while ( ( strtok( NULL, aDelimiter ) ) != NULL )
	{
		sNumOfResult ++;
	}
	
	return sNumOfResult;
}

void makeArgv( char ***aArgv, 
			  char *aOrigText,
			  const int aArgc,
			  const char *aDel )
{
	int i = 0;

	*aArgv = (char**)malloc( sizeof(char) * ( aArgc + 1 ) );

	(*aArgv)[0] = strtok( aOrigText, aDel );

	for ( i = 1; i < aArgc; i++ )
	{
		(*aArgv)[i] = strtok( NULL, aDel );
	}

	(*aArgv)[i] = NULL;
}

int makeArg( const char *aOrigText, 
			  const char *aDelimiter, 
			  char ***aArgv )
{
	char *sStrResult = NULL;
	const char *sCpyText;
	char *sNew = NULL;
	int sNumOfResult = 1;
	int len = 0;
	int sRC = 0;

	if ( aOrigText == NULL || aDelimiter == NULL)
	{
		fprintf( stderr, "string is NULL\n" );
		return -1;
	}
	else
	{
		/* do nothing */
	}

	sCpyText = aOrigText + strspn( aOrigText, aDelimiter ); 
	sNew = (char*)malloc( strlen( sCpyText ) + 1 );
	strcpy( sNew, sCpyText );

	sNumOfResult = makeArgc( sNew, aDelimiter );

	strcpy( sNew, sCpyText );

	makeArgv( aArgv,
			  sNew, 
			  sNumOfResult, 
			  aDelimiter );

	return sNumOfResult;
}


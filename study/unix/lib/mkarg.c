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

void makeArg( char ***aArgv, 
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

int makeArgv( const char *aOrigText, 
			  const char *aDelimiter, 
			  char ***aArgv,
			  int  *aArgc )
{
	char *sStrResult = NULL;
	const char *sCpyText;
	char *sNew = NULL;
	int sNumOfResult = 1;
	int len = 0;
	int sRC = 0;

	if ( aOrigText == NULL || aDelimiter == NULL)
	{
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
	*aArgc = sNumOfResult;	

	strcpy( sNew, sCpyText );

	makeArg( aArgv,
			  sNew, 
			  sNumOfResult, 
			  aDelimiter );

	return 0;
}


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int freeMemory( char **aText );
void error_handling( char *msg );

int makeArgc( const char *aOrigText, const char *aDelimiter )
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
			  const char *aOrigText,
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

	return sRC;
}

int freeMemory( char **aText )
{
	if ( aText != NULL )
	{
		if ( *aText != NULL )
			free( *aText );
		
		return 0;
	}
	else
	{
		return -1;
	}
}

int main( int argc, char *argv[] )
{
	int sArgc = 0;
	int i = 0;
	int sRC = 0;
	char del[] = " ";
	char **sArgvReturn = NULL;

	if ( argc != 2 )
	{
		error_handling( "Argument Error" );
		exit(1);
	}
	else
	{
		/* do nothing */
	}

	sRC = makeArgv( argv[1], 
   	     		 	del, 
				 	&sArgvReturn,
				 	&sArgc );

	if ( sRC == -1 )
	{
		error_handling( "Error" );
		exit(1);
	}
	else
	{
		printf ( "Return argc : %d\n", sArgc );
		for ( i = 0; i < sArgc; i++ )
		{
			printf ( "Return Argv[%d] : %s\n", i, sArgvReturn[i] );
		}
	}

	freeMemory( sArgvReturn );

	return 0;
}

void error_handling( char *msg )
{
	fputs( msg, stderr );
	fputc( '\n', stderr );
}

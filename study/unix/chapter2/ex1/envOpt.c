#include "../../ide.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define ALLOC_SIZE 8

extern char **environ;                                             
static char **gNewEnv;
static int gSizeOfEnv;

void printGetEnv()
{
	int i = 0;

	for ( i = 0; gNewEnv[i] != NULL; i++ )
	{
		printf( "env[%d] : %s\n", i, gNewEnv[i] );
	}
}

int copyEnv()
{
    int i = 0;

    for( i = 0; environ[i] != NULL; i++ ) {}

    gSizeOfEnv = i;
    
    gNewEnv = (char**)malloc( sizeof(char**) * ( gSizeOfEnv + 1 ) );

    for( i = 0; i < gSizeOfEnv; i++ )
    {
        gNewEnv[i] = environ[i];
    }
    gNewEnv[gSizeOfEnv] = NULL;

    return IDE_SUCCESS;

}

int freeEnv()
{
    if ( gNewEnv != NULL )
    {
        free(gNewEnv);
        gNewEnv = NULL;
        gSizeOfEnv = 0;
    }
    else
    {
        /* do nothing */
    }

	return IDE_SUCCESS;
}

int isEnv( const char *aOpt, int *idx )
{
	int i = 0;
	int len = strlen( aOpt );
	char *sCpyOpt; 
	char *sOpt;
	char *sCpyEnv;
	char *sEnvOpt;

	sCpyOpt = (char*)malloc( sizeof(char*) * ( len + 1 ) );

	strcpy( sCpyOpt, aOpt );
	sCpyOpt[len] = '\0';

	sOpt = strtok( sCpyOpt, "=" );

	IDE_TEST( sOpt == NULL );
	for( i = 0; gNewEnv[i] != NULL; i++ )
	{
		len = strlen( gNewEnv[i] );
		sCpyEnv = (char*)malloc( sizeof(char*) * ( len + 1 ) );

		strcpy( sCpyEnv, gNewEnv[i] );
		sCpyEnv[len] = '\0';
		
		sEnvOpt = strtok( sCpyEnv, "=" );
	
		if ( sEnvOpt != NULL )
		{
			if ( strcmp( sOpt, sEnvOpt ) == 0 )
			{
				*idx =  i;

				free(sCpyOpt);
				free(sCpyEnv);
				return IDE_SUCCESS;
			}
			else
			{
				/* do nothing */
			}
		}
		else
		{
			/* do nothing */
		}
		free(sCpyEnv);
	}

	EXCEPTION_END

	*idx = i;
	
	free(sCpyOpt);
	
	return IDE_FAILURE;
}

int ignoreOpt( const char **argv, const int argc )
{
	int i = 2;
	int sRC = 0;
	int idx = 0;
	int len = 0;
	char *cmd = NULL;
	char *sOptName = NULL;

	for( i = 2; argv[i] != NULL; i++ )
	{
		len = strlen( argv[i] );

		sRC = isEnv( argv[i], &idx );

		if ( sRC == IDE_TRUE )
		{
			gNewEnv[idx] = (char*)malloc( len + 1 );
			strcpy( gNewEnv[idx], argv[i] );
			gNewEnv[idx][len] = '\0';
		}
		else
		{
			if ( idx >= gSizeOfEnv )
			{
				gNewEnv = (char**)realloc( gNewEnv,  gSizeOfEnv + sizeof(char) * ALLOC_SIZE );

				gSizeOfEnv += ALLOC_SIZE;
			}
			else
			{
				/* do nothing */
			}
			
			gNewEnv[idx] = (char*)malloc( len + 1 );
i			strcpy( gNewEnv[idx], argv[i] );
			gNewEnv[idx][len] = '\0';

			gNewEnv[idx + 1] = NULL;
		}
	}

	printGetEnv();

	return IDE_SUCCESS;
}

int utilityOpt( const char **argv, const int argc )
{
	int i = 2;
	int sRC = 0;
	int len = 0;
	char *cmd = NULL;

	for( i = 2; argv[i] != NULL; i++ )
	{
		len = strlen( argv[i] ) + strlen( "evn " );

		cmd = (char*)malloc( sizeof(char*) * ( len + 1 ) );
		strcpy( cmd, "env " );
		strcat( cmd, argv[i] );
		cmd[len] = '\0';

		printf( "cmd[%d] : %s\n", i, cmd );

		sRC = system(cmd);
		IDE_TEST_RAISE( sRC == -1, EXE_ERROR );

		free(cmd);
	}

	return IDE_SUCCESS;

	IDE_EXCEPTION( EXE_ERROR )
	{
		printf( "%s Execute Error\n", cmd );
		free(cmd);
	}
	EXCEPTION_END

	return IDE_FAILURE;}

int getOpt( const char *opt )
{
	if ( strcmp( opt, "-i" ) == 0 )
	{
		return 1;
	}
	else if ( strcmp( opt, "-u" ) == 0 )
	{
		return 2;
	}

	return IDE_FAILURE;
}

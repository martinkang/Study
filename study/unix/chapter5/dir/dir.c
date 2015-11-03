#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include "stack.h"
#include "queue.h"

int BFS_S( int aDepth, char *aPath )
{
	int sRC = 0;
	int sIdx = 0;
	int isDir = 0;
	int D = 1;
	char sPath[255] = {0, };
	char *sData = NULL;
	DIR *dirp = NULL;
	struct dirent *redirP = NULL;
	struct dirent entry;
	struct stat stat_buf;

	que qu;

	q_init ( &qu );

	q_push ( &qu, aPath );

	while ( qu.mNum_que > 0 )
	{
		q_pop ( &qu, &sData );

		printf( "%s/\n", sData );
		dirp = opendir( sData );
		if ( dirp == NULL )
		{
			return -1;
		}

		sRC = readdir_r( dirp, &entry, &redirP );
		while ( redirP != NULL )
		{
			if ( redirP->d_name[0] != '.' )
			{
				sprintf( sPath, "%s/%s", sData, redirP->d_name );
				stat( sPath, &stat_buf );
				isDir = S_ISDIR( stat_buf.st_mode );

				if ( isDir == 1 )
				{
					q_push( &qu, sPath );	
				}
				else
				{	
					printf( "%s\n", redirP->d_name );
				}
			}
			sRC = readdir_r( dirp, &entry, &redirP );
		}

		closedir( dirp );

		D++;
	}

	q_finalize( &qu );
}

int DFS_S( int aDepth, char *aPath )
{
	int sRC = 0;
	int sIdx = 0;
	int isDir = 0;
	char sPath[255] = {0, };
	DIR *dirp = NULL;
	struct dirent *redirP = NULL;
	struct dirent entry;
	struct stat stat_buf;

	dirp = opendir( aPath );
	if ( dirp == NULL )
	{
		return -1;
	}
	
	sRC = readdir_r( dirp, &entry, &redirP );
	while ( redirP != NULL )
	{
		if ( redirP->d_name[0] != '.' )
		{
			for ( sIdx = 0; sIdx < aDepth; sIdx++ )
				printf( "\t" );
	
			sprintf( sPath, "%s/%s", aPath, redirP->d_name );
			stat( sPath, &stat_buf );
			isDir = S_ISDIR( stat_buf.st_mode );

			if ( isDir == 1 )
			{
				printf( "%s/\n", sPath );
				DFS_S( aDepth + 1, sPath );
			}
			else
			{
				printf( "%s\n", redirP->d_name );
			}
		}
		sRC = readdir_r( dirp, &entry, &redirP );
	}

	closedir( dirp );
}

int DFS( char *aPath )
{
	int sDepth = 0;

	DFS_S ( 1, aPath );

	return 1;
}

int BFS( char *aPath )
{
	int sDepth = 0;

	printf( "\nsearch path : %s\n", aPath );
	BFS_S ( 1, aPath );

	return 1;
}

int main( int argc, char *argv[])
{
	int sRC = 0;
	int sSwitch = 0;

	char *sPath = NULL;

	if ( argc < 2 || argc > 3 )
	{
		fprintf( stderr, "Argument Error\n" );
		return -1;
	}

	sSwitch = atoi( argv[1] );
	if ( sSwitch > 2 || sSwitch == 0 )
	{
		fprintf( stderr, "Argument[1] is Error\n" );
		return -1;
	}

	if ( argc == 3 )
	{
		sPath = argv[2];
	}
	else
	{
		sPath = ".";
	}

	if ( sSwitch == 1 ) //DFS
	{
		sRC = DFS( sPath );
		if ( sRC == -1 )
		{
			fprintf( stderr, "DFS is Error\n" );
		}
	}
	else //BFS
	{
		sRC = BFS( sPath );
		if ( sRC == -1 )
		{
			fprintf( stderr, "BFS is Error\n" );
		}
	}

	return 0;
}

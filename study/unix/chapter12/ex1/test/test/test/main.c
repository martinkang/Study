#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <dirent.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "que.h"
#include "dir.h"
#include "../../ide.h"

#define BUF_SIZE   1024
#define MAX_THREAD 8
#define DELIMITER "/"

que_t *gQue;

static sig_atomic_t queIsEnable = 0;
static sig_atomic_t doneflag = 0;

void *copyFiles( void *arg )
{
	int sRC  = 0;
	int step = 0;
	copyInfo_t *cur;

	char pathIn[255] = {0, };
	char pathOut[255] = {0, };
	FILE *fdin = NULL;
	FILE *fdout = NULL;

	char buf[BUF_SIZE] = {0, };

	while ( doneflag == 0 )
	{
		while ( queIsEnable == 0 );
		queIsEnable = 0;
		sRC = dequeue( (void**)&cur );
		if ( sRC == -1 )
		{
			doneflag = 1;
			pthread_exit( NULL );
		}

		queIsEnable = 1;

		step = 1;

		if ( cur->type == 0 )//folder
		{
			sRC = mkdir( cur->destPath, 0755 );
			if ( sRC == -1 && errno != EEXIST )
			{
				IDE_TEST( 1 );
			}
		}
		else
		{
			snprintf( pathIn, 255, "%s/%s", cur->srcPath, cur->fname );
			snprintf( pathOut, 255, "%s/%s", cur->destPath, cur->fname );

			fdin = fopen( pathIn, "r" );
			if ( fdin == NULL && errno != EEXIST )
			{
				IDE_TEST( 1 );
			}
			else if ( fdin == NULL && errno == EEXIST )
			{
			}
			else if ( fdin != NULL )
			{
				step = 2;

				fdout = fopen( pathOut, "w+" );
				if ( fdout == NULL && errno != ENOENT )
				{
					IDE_TEST( 1 );
				}
				else if ( fdout == NULL && errno == ENOENT )
				{
					sRC = mkdir( cur->destPath, 0755 );
					if ( sRC == -1 && errno != EEXIST )
					{
						IDE_TEST( 1 );
					}
					fdout = fopen( pathOut, "w+" );
					if ( fdout == NULL )
					{
						IDE_TEST( 1 );
					}
				}

				step = 3;

				while ( fgets( buf, BUF_SIZE, fdin )  != NULL )
				{
					sRC = fputs( buf, fdout );
					if ( sRC < 0 )
						IDE_TEST( 1 );
				}

				step = 2;
				fclose( fdout );

				step = 1;
				fclose( fdin );
			}
		}
		step = 0;
		free( cur->fname );
		free( cur->srcPath );
		free( cur->destPath );
		free( cur );
	}

	pthread_exit( NULL );

	EXCEPTION_END
	
	switch ( step )
	{
		case 3 :
			fclose( fdout );
		case 2 :
			fclose( fdin );
		case 1 :
			free( cur->fname );
			free( cur->srcPath );
			free( cur->destPath );
			free( cur );
			break;
	default :
		break;
	}

	pthread_exit( NULL );
}

int main( int argc, char *argv[] )
{
	int sRC = 0;

	int i = 0;
	
	int len = 0;
	char pathIn[255] = {0, };
	char pathOut[255] = {0, };

	int ch[MAX_THREAD] = {0, };
	pthread_t th[MAX_THREAD];

	if ( argc != 3 )
	{
		fprintf( stderr, "Argument error\n" );
		return -1;
	}

	initQue();

	len = strlen( argv[1] );
	strcpy( pathIn, argv[1] );
	if (  pathIn[len - 1]  == '/' )
		pathIn[len - 1] = '\0';

	len = strlen( argv[2] );
	strcpy( pathOut, argv[2] );
	if ( pathOut[len - 1] == '/' )
		pathOut[len - 1] = '\0';

	sRC = scanFile( pathIn, pathOut );
	if ( sRC == -1 )
	{
		IDE_TEST( 1 );
	}

	queIsEnable = 1;
	
	for ( i = 0; i < MAX_THREAD; i++ )
	{
		sRC = pthread_create( &( th[i] ), NULL, copyFiles, NULL );
		if ( sRC == -1 )
		{
			perror( "thread create error" );
		}
		else
		{
			ch[i] = 1;
		}
	}

	for ( i = 0; i < MAX_THREAD; i++ )
	{
		if ( ch[i] == 1 )
		{
			sRC = pthread_join( th[i], NULL );
			if ( sRC == -1 )
			{
				perror( "thread join error" );
			}
		}
	}
	fprintf( stderr, "Copy complete\n" );

	return 0;

	EXCEPTION_END;

	return -1;
}

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "que.h"
#include "dir.h"
#include "../../ide.h"

extern que_t gQue;

typedef struct folders
{
	char *mFname;
	char *mSrc;
	char *mDest;
	struct folders *mNext;
} folder_t;

folder_t *mallocNewFolder( const char *aPath, const char *aSrc, const char *aDest )
{
	folder_t *sNewFolder = NULL;
	sNewFolder = (folder_t*)malloc( sizeof(folder_t) );
	sNewFolder->mFname = (char*)malloc( sizeof( char ) * strlen( aPath ) );
	sNewFolder->mSrc = (char*)malloc( sizeof( char ) * strlen( aSrc ) );
	sNewFolder->mDest = (char*)malloc( sizeof( char ) * strlen( aDest ) );

	sNewFolder->mNext = NULL;
	strcpy( sNewFolder->mFname, aPath );
	strcpy( sNewFolder->mSrc, aSrc );
	strcpy( sNewFolder->mDest, aDest );

	return sNewFolder;
}

copyInfo_t *mallocNewCopyInfo( const char *name, const char *pathIn, const char *pathOut )
{
	copyInfo_t *cpInfo = NULL;
	cpInfo = (copyInfo_t*)malloc( sizeof(copyInfo_t) );
	cpInfo->fname = (char*)malloc( sizeof( char ) * strlen( name ) );		
	cpInfo->srcPath = (char*)malloc( sizeof( char ) * strlen( pathIn ) );
	cpInfo->destPath = (char*)malloc( sizeof( char ) * strlen( pathOut ) );

	return cpInfo;

}
int searchDir( const char *pathIn, const char *pathOut )
{
	int sRC = 0;
	int step = 0;

	DIR *sIn = NULL;
	
	int type = 0;
	char path[255] = {0, };
	char path2[255] = {0, };

	struct stat st;
	struct dirent *rentP = NULL;
	copyInfo_t *cpInfo = NULL;

	folder_t *sNewFolder = NULL;
	folder_t *sFol_Head = NULL;
	folder_t *sFol_Tail = NULL;
	folder_t *sCur = NULL;

	sIn = opendir( pathIn );
	if ( sIn == NULL )
	{
		fprintf( stderr, "%s ", pathIn );
		perror( "Source Directory open error" );
		IDE_TEST( 1 );
	}
	step = 1;

	while ( ( rentP = readdir( sIn ) ) != NULL )
	{
		if ( ( strcmp( rentP->d_name, "/" ) != 0 ) && 
				( strcmp( rentP->d_name, ".." ) != 0 ) &&
				( strcmp( rentP->d_name, "." ) != 0 ) )
		{			
			cpInfo = (copyInfo_t*)malloc( sizeof(copyInfo_t) );
			step = 2;

			snprintf( path, 255, "%s/%s", pathIn, rentP->d_name	);
			sRC = stat( path, &st );
			if ( sRC == -1 )
			{
				perror( "Is DIR error" );
				IDE_TEST( 1 );
			}
			if ( S_ISDIR( st.st_mode ) == 0 )
			{					
				type = 1;
			}
			else // forlder
			{
				sNewFolder = mallocNewFolder( rentP->d_name, pathIn, pathOut );
				step = 3;
				if ( sFol_Tail == NULL )
				{
					sFol_Tail = sNewFolder;
					sFol_Head = sFol_Tail;
				}
				else
				{
					sFol_Tail->mNext= sNewFolder;
					sFol_Tail = sNewFolder;
				}
				type = 0;
			}

			cpInfo = mallocNewCopyInfo( rentP->d_name, pathIn, pathOut );
			step = 4;

			cpInfo->type = type;

			strcpy( cpInfo->fname, rentP->d_name );
			strcpy( cpInfo->srcPath, pathIn );
			strcpy( cpInfo->destPath, pathOut );
	
			sRC = queue( (void*)cpInfo );
			if ( sRC == -1 )
				IDE_TEST( 1 );
		}
	}

	step = 0;
	closedir( sIn );

	sCur = sFol_Head;
	while( sCur != NULL )
	{
		snprintf( path, 255, "%s/%s", sCur->mDest, sCur->mFname );
		snprintf( path2, 255, "%s/%s", sCur->mSrc, sCur->mFname );

		searchDir( path2, path );

		sFol_Head = sFol_Head->mNext;
		free( sCur->mFname );
		free( sCur->mSrc );
		free( sCur->mDest );
		free( sCur );

		sCur = sFol_Head;
	}

	return sRC;

	EXCEPTION_END
	
	switch ( step )
	{
		case 4:
			free( cpInfo );
		case 3:
			free( sNewFolder );
		case 2:
			free( cpInfo );
		case 1:
			closedir( sIn );
		default :
			break;
	}

	return -1;
}

int scanFile( const char *pathIn, const char *pathOut )
{
	int sRC = 0;
	int step = 0;

	int i = 0;
	int idx = 0;
	char *cpStr = NULL;
	char *bufCur = NULL;
	struct stat sStat;
	copyInfo_t *cpInfo = NULL;

	sRC = stat( pathIn, &sStat );
	if ( sRC == -1 )
		IDE_TEST( 1 );

	if ( S_ISDIR( sStat.st_mode ) > 0 )
	{
		sRC = searchDir( pathIn, pathOut );
		if ( sRC == -1 )
			IDE_TEST( 1 );

		fprintf( stderr, "Directory Scan Complete\n" );
	}
	else
	{
		cpStr = (char*)malloc( sizeof( char ) * strlen( pathIn ) );
		step = 1;
		strcpy( cpStr, pathIn );
		
		for ( i = 0; i < strlen( pathIn ); i++ )
		{
			if ( cpStr[i] == '/' )
			{
				idx = i;
			}
		}

		if ( idx > 0 )
		{
			cpStr[idx] = 0;
			bufCur = cpStr + idx + 1;
		}

		cpInfo = (copyInfo_t*)malloc( sizeof(copyInfo_t) );
		cpInfo->fname = (char*)malloc( sizeof( char ) * strlen( bufCur ) );
		cpInfo->srcPath = (char*)malloc( sizeof( char ) * strlen( cpStr ) );
		cpInfo->destPath = (char*)malloc( sizeof( char ) * strlen( pathOut ) );
		cpInfo->type = 1;
		step = 2;

		strcpy( cpInfo->fname, bufCur );
		strcpy( cpInfo->srcPath, pathIn );
		strcpy( cpInfo->destPath, pathOut );

	}

	return sRC;

	EXCEPTION_END

	switch ( step )
	{
		case 2:
			free( cpInfo->fname );
			free( cpInfo->srcPath );
			free( cpInfo->destPath );
			free( cpInfo );
		case 1:
			free( cpStr );
			break;
		default :
			break;
	}

	return -1;
}


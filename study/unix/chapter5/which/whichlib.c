#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include "which.h"

int sepPath( const char *origPath, char ***aPath, int *aNumPath )
{
	int slen = 0;
	int sNum = 1;
	int sIdx = 0;

	char *sTempPath = NULL;
	char **sPath;
	char *cpyPath = NULL;

	if ( origPath == NULL )
	{
		return -1;
	}

	slen = strlen( origPath );
	cpyPath = (char*)malloc( sizeof(char) * (slen + 1 ) );
	strcpy( cpyPath, origPath );
	cpyPath[slen] = '\0';

	strtok( cpyPath, ":" ); 
	while ( strtok( NULL, ":" ) != NULL )
	{
		sNum ++;
	}

	*aNumPath = sNum;

	sPath = (char**)malloc( sizeof(char*) * ( sNum + 1 ) );

	if ( sNum == 1 )
	{
		slen = strlen( origPath );
		sPath[0] = (char*)malloc( sizeof(char) * (slen + 1 ) );
		
		strcpy( sPath[0], origPath );
		sPath[1] = NULL;
	
		*aPath = sPath;
		return 1;
	}

	strcpy( cpyPath, origPath );
	cpyPath[slen] = '\0';

	sTempPath = strtok( cpyPath, ":" );
	slen = strlen( sTempPath );

	sPath[sIdx] = (char*)malloc( sizeof(char) * (slen + 1 ) );
	strcpy( sPath[sIdx], sTempPath );
	sPath[sIdx][slen] = '\0';
	sIdx++;

	while ( ( sTempPath = strtok( NULL, ":" ) ) != NULL )
	{
		slen = strlen( sTempPath );
		sPath[sIdx] = (char*)malloc( sizeof(char) * (slen + 1 ) );
		strcpy( sPath[sIdx], sTempPath );
		sPath[sIdx][slen] = '\0';
		sIdx++;
	}

	sPath[sIdx] = NULL;

	*aPath = sPath;
	
	return 1;
}

int haveFile( const char *aFname, const char *aDir )
{
	DIR *dirP = NULL;
	struct dirent *redirP;

	dirP = opendir( aDir );
	if ( dirP == NULL )
		return -1;
	
	while( ( redirP = readdir( dirP ) ) != NULL )
	{
		if ( strcmp( aFname, redirP->d_name ) == 0 )
		{
			return 1;
		}
	}
	
	closedir( dirP );

	return -1;
}

int findFile( const char *aFname, const char **aPath, int *aFindPath )
{
	int sRC = 0;
	int sIdx = 0;
	int sNum = 0;

	for ( sIdx = 0; aPath[sIdx] != NULL; sIdx++ )
	{
		sRC = haveFile( aFname, aPath[sIdx] );
		if ( sRC == 1 )
		{
			aFindPath[sIdx] = 1;
			sNum++;
		}
	}
	
	return sNum;
}

int checkexecutable( const char *aFname, const char *aPath )
{
	struct stat statbuf;
	int sUid = getuid();
	int sGid = getegid();

	if ( stat( aPath, &statbuf ) == -1 )
		return -1;

	printf( "UID : %d\nGID : %d\nmyUID : %d\nmyGID : %d\n", statbuf.st_uid, statbuf.st_gid, sUid, sGid );

	if ( statbuf.st_uid == sUid || statbuf.st_gid == sGid )
		return 1;

	return 0;
}

void error_handling( const char *msg )
{
	fputs( msg, stderr );
	fputc( '\n', stderr );
}

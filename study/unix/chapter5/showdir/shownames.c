#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct subdi
{
	char *d_name;
	struct subdi *mNext;
}subD;

void freeNode( subD *aHeadD )
{
	subD *sPtr = aHeadD;
	subD *sDel = NULL;

	while( sPtr == NULL )
	{
		sDel = sPtr;
		sPtr = sPtr->mNext;
		
		if ( sDel != NULL )
			free( sDel );
	}
}
subD* newNode( int alen, char *aDir )
{
	subD *newNode = (subD*)malloc( sizeof(subD) );
	
	newNode->d_name = (char*)malloc( sizeof(char) * ( alen + 1 ) );
	strcpy( newNode->d_name, aDir );

	(newNode->d_name)[ alen ] = '\0';
	newNode->mNext = NULL;

	return newNode;
}

int findAll( int D, char *aDir )
{
	struct dirent *dip;
	DIR *dirp;
	DIR *dirps;
	int slen = 0;
	int sRC = 0;
	char *sDir = NULL;
	char buf[1024] = {0, };

	subD *sCurD = NULL;
	subD *sHeadD = NULL;

	getcwd( buf, 1024 );
	printf( "\ncurrent dir : %s\n", buf );				
	
	if ( (dirp = opendir(aDir)) == NULL)// && errno != ENOTDIR )
	{
		return 1;
	}

	printf ( "\nDIR : %s\n", aDir );
	while( (dip = readdir(dirp)) != NULL)
	{
		printf( "%s\n", dip->d_name);
		slen = strlen(dip->d_name);

		if ( sHeadD == NULL )
		{
			sHeadD = newNode( slen, dip->d_name );
			sCurD = sHeadD;
		}
		else
		{
			sCurD->mNext = newNode( slen, dip->d_name );
			sCurD = sCurD->mNext;
		}
	}

	while((closedir(dirp) == -1 ) && (errno == EINTR));

	if ( sHeadD != NULL )
	{
		sCurD = sHeadD;
		while ( sCurD != NULL )
		{
			sDir = sCurD->d_name;
			sCurD = sCurD->mNext;
			
			if ( sDir[0] != '.' )
			{
				if ( D > 0 )
				{	
					sRC =chdir( aDir );
					if ( sRC == 0 )
					{
						sRC = findAll( D + 1, sDir );	
						chdir( ".." );
					}
				}
				else
				{
					sRC = findAll( D + 1, sDir );	
				}
				
			}
		}
	}

	freeNode( sHeadD );
	sCurD = NULL;
}

int main( int argc, char *argv[] )
{
	int sRC = 0;

	if ( argc != 1 )
	{
		fprintf( stderr, "Usage: %s directory_name\n", argv[0] );
		return 1;
	}

	sRC = findAll( 0, "." );

	return 0;
}

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>
#include "license.h"

#define PERMS (mode_t)(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH )

char gSem_Name[] = "license";

static sem_t *gLicenseSem;
static int gIsInit = 0;
static int gNumLicense = 3;


int getlicense( void )
{
	int sRC = 0;

	sRC = sem_wait( gLicenseSem );
	return sRC;
}

int returnlicense( void )
{
	int sRC = 0;

	sRC = sem_post( gLicenseSem );
	return sRC;
}

int initlicense( void )
{
	gLicenseSem = sem_open( gSem_Name, O_CREAT | O_EXCL, PERMS, gNumLicense );
	if ( gLicenseSem == SEM_FAILED && 
			errno == EEXIST )
	{
		gLicenseSem = sem_open( gSem_Name, O_CREAT );
		if ( gLicenseSem == SEM_FAILED )
			return errno;
	}
	else if ( gLicenseSem == SEM_FAILED && errno != EEXIST )
	{
		return errno;
	}

	gIsInit = 1;

	return 0;


}

int finallicense( void )
{
	int sRC = 0;

	gIsInit = 0;

	sRC = sem_close( gLicenseSem );
	if ( sRC != 0 )
		return sRC;

	sRC = sem_unlink( gSem_Name );
	return sRC;
}

int getLicenseNum( void )
{
	return gNumLicense;
}

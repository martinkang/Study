#include <string.h>
#include "uici.h"
#include "uiciname.h"

#ifndef REENTRANCY

#define REENTRANCY REENTRANT_NONE

#endif

#if REENTRANCY==REENTRANT_MUTEX

#include <pthread.h>
static pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;

#endif

#if REENTRANCY==REENTRANT_NONE

int name2addr( const char *name, in_addr_t *addrp )
{
	struct hostent *hp;

	if ( isdigit( (int)(*name) ) == 1 )
	{
		*addrp = inet_addr(name);
	}
	else
	{
		hp = gethostbyname( name );
		if ( hp == NULL )
			return -1;

		memcpy( (char*)addrp, hp->h_addr_list[0], hp->h_length );
	}

	return 0;
}

void addr2name( struct in_addr addr, char *name, int namelen )
{
	struct hostent *hp;
	
	hp = gethostbyaddr( (char*)&addr, 4, AF_INET );
	if ( hp == NULL )
	{
		strncpy( name, inet_ntoa(addr), namelen - 1 );
	}
	else
	{
		strncpy( name, hp->h_name, namelen - 1 );
	}

	name[namelen - 1] = 0;
}
#elif REENTRANCY==REENTRANT_MUTEX

int name2addr( const char *name, in_addr_t *addrp )
{
	int sRC = 0;
	struct hostent *hp;

	if ( isdigit( (int)(*name) ) == 1 )
	{
		*addrp = inet_addr(name);
	}
	else
	{
		sRC = pthread_mutex_lock( &gMutex );
		if ( sRC == -1 )
			return -1;

		hp = gethostbyname( name );
		if ( hp == NULL )
		{
			pthread_mutex_lock( &gMutex );
			return -1;
		}

		memcpy( (char*)addrp, hp->h_addr_list[0], hp->h_length );
		pthread_mutex_lock( &gMutex );
	}

	return 0;
}

void addr2name( struct in_addr addr, char *name, int namelen )
{
	int sRC = 0;
	struct hostent *hp;

	sRC = pthread_mutex_lock( &gMutex );
	if ( sRC == -1 )
	{
		strcpy( name, inet_ntoa(addr), namelen - 1 );
		name[namelen - 1] = 0;
		return;
	}
	hp = gethostbyaddr( (char*)&addr, 4, AF_INET );
	if ( hp == NULL )
	{
		strcpy( name, inet_ntoa(addr), namelen - 1 );
	}
	else
	{
		strcpy( name, hp->h_name, namelen - 1 );
	}
	
	pthread_mutex_lock( &gMutex );

	name[namelen - 1] = 0;
}endif

#endif

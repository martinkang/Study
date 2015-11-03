#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include "jobs.h"

static joblist_t *jobHead = NULL;
static joblist_t *jobTail = NULL;
static int numJob = 0;

void finalize()
{
	joblist_t *cur = NULL;
	joblist_t *prev = NULL;
	if ( jobHead != NULL )
	{
		prev = jobHead;
		free( prev );
		jobHead = NULL;

		cur = jobHead->next;	
	}

	while ( cur != NULL )
	{
		prev = cur;
		cur = cur->next;

		kill( cur->job, SIGKILL );
		free( prev );
		prev = NULL;

		wait( NULL );
	}
}

void initList( void )
{
	joblist_t *newJobH = NULL;

	newJobH = ( joblist_t * )malloc( sizeof( joblist_t ) );

	jobHead = newJobH;
}

joblist_t *findJob( int job )
{
	joblist_t *cur = NULL;
	
	if ( jobTail == NULL )
	{
		return NULL;
	}

	cur = jobHead->next;

	while( cur != NULL )
	{
		if ( cur->job == job )
		{
			return cur;
		}
		else
		{
			cur = cur->next;
		}
	}

	return NULL;
}

int addjob( pid_t pgid, char *cmd, job_status_t status )
{
	int len = 0;
	joblist_t *newJob = NULL;

	len = strlen( cmd );

	newJob = ( joblist_t *)malloc( sizeof( joblist_t ) );
	newJob->cmdstr = ( char * )malloc( sizeof( char ) * ( len  ) );
	newJob->pgid = pgid;
	strcpy( newJob->cmdstr, cmd );
	newJob->jobstat = status;
	newJob->next = NULL;

	if ( jobTail == NULL )//first job
	{
		if ( jobHead == NULL )
			initList();

		jobTail = newJob;
		jobHead->next = jobTail;
	}
	else
	{
		jobTail->next = newJob;
		jobTail = newJob;
	}

	numJob++;
}

int deljob ( int job )
{
	int sRC = 0;
	joblist_t *cur;
	joblist_t *prev;

	if ( jobTail == NULL )
	{
		fprintf( stderr, "job list is empty\n" );
		return -1;
	}

	prev = jobHead;
	cur = jobHead->next;

	while( cur != NULL )
	{
		if ( cur->job == job )
		{
			sRC = 1;	
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}

	if ( sRC == 1 )
	{
		if ( cur == jobTail )
		{
			if ( prev != jobHead )
			{
				jobTail = prev;
			}
			else
			{
				jobTail = NULL;
				free( jobHead );
			}
		}
		prev->next = cur->next;
		
		free( cur );
		cur = NULL;
		
		fprintf( stderr, "job %d is deleted\n", job );
		return 0;
	}
	else
	{
		fprintf( stderr, "job %d is not here\n", job );
		return -1;
	}
}

int showjobs( void )
{
	joblist_t *cur;

	if ( jobTail == NULL )
	{
		fprintf( stderr, "job list is empty\n" );
		return -1;
	}

	cur = jobHead->next;
	while ( cur != NULL )
	{
		printf( " [job : %d] status : %d\tpgid : %d\tcmd : %s\n", cur->job, cur->jobstat, cur->pgid, cur->cmdstr );
		cur = cur->next;
	}

	printf( "\n" );
	return 0;
}

int setstatus( int job, job_status_t status )
{
	joblist_t *cur = NULL;

	cur = findJob( job );

	if ( cur == NULL )
	{
		fprintf( stderr, "job %d is not here\n", job );
		return -1;
	}
	
	cur->jobstat = status;
	return 0;
}

int getstatus( int job, job_status_t *pstatus )
{
	joblist_t *cur = NULL;

	cur = findJob( job );

	if ( cur == NULL )
	{
		fprintf( stderr, "job %d is not here\n", job );
		return -1;
	}
	
	*pstatus = cur->jobstat;
	return 0;
}

pid_t getPGid( int job )
{
	joblist_t *cur = NULL;

	cur = findJob( job );

	if ( cur == NULL )
	{
		fprintf( stderr, "job %d is not here\n", job );
		return -1;
	}

	return cur->pgid;
}


int getNumJobs( void )
{
	return numJob;
}

int getlargest( void )
{
	int max = -1;
	joblist_t *cur = NULL;

	if ( jobTail == NULL )
	{
		fprintf( stderr, "job list is empty\n" );
		return -1;
	}

	cur = jobHead->next;
	while( cur != NULL )
	{
		if ( cur->job > max )
			max = cur->job;

		cur = cur->next;
	}

	return max;
}


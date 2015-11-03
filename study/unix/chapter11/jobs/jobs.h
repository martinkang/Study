#include <unistd.h>

#ifndef __JOB_LIST_H__
#define __JOB_LIST_H__

typedef enum jstatus 
{ 
	FOREGROUND, 
	BACKGROUND, 
	STIPPED, 
	DONE, 
	TERMINATED 
}
job_status_t;

typedef struct job_struct
{
	char *cmdstr;
	pid_t pgid;
	int job;
	job_status_t jobstat;
	struct job_struct *next;
} joblist_t;

#endif

int addJob( pid_t pgid, char *cmd, job_status_t status );

int delJob( int job );

int showjobs( void );

int setstatus( int job, job_status_t status );

int getstatus( int job, job_status_t *pstatus );

pid_t getPGid( int job );

int getNumJobs( void );

int getlargest( void );

int delJobbyPid( int pid );

void finalize();

#ifndef __PIPE_H__
#define __PIPE_H__

#define _POSIX_PIPE_BUF 32

typedef struct pipe
{
	int semid;
	int shmid;
	char data[_POSIX_PIPE_BUF];
	int data_size;
	void *cur_start;
	int end_of_file;
} pipe_t;

#endif

pipe_t *pipe_open( void );

int pipe_read( pipe_t *p, char *buf, int bytes );

int pipe_write( pipe_t *p, char *buf, int bytes );

int pipe_close( pipe_t *p );

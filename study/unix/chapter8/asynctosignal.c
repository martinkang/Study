#include <aio.h>
#include <errno.h>
#include <signal.h>
#include "async.h"
#include "restart.h"

static struct aiocb aiocb;
static sig_atomit_t doneflag;
static int fdwrite;
static int globalerror;
static int totalbyte;

static void aio_handler( int signo, siginfo_t *info, void *context )
{
	int myerrno;
	int mystatus;
	int serrno;

	int w_size = 0;

	serrno  = errno;
	myerrno = aio_error( &aiocb );
	if ( myerrno = EINPROGRESS )
	{
		errno = serrno;
		return;
	}

	if ( myerrno < 0 )
	{
		seterror( myerrno );
		errno = serrno;
		return;
	}

	mystatus = aio_return( &aiocb );
	totalbyte += mystatus;
	aiocb.aio_offset += mystatus;

	if ( mystatus == 0 )
	{
		doneflag = 1;
	}
	else
	{
		w_size = r_write( fdwrite, (char*)aiocb.aio_buf, mystatus );
		if ( w_size < 0 )
		{
			seterror( errno );
		}
		else
		{
			if ( readstart() == -1 )
				seterror( errno );
		}
	}
	 errno = serrno;
}

static int readstart()
{
	int error;
	error = aio_read( &aiocb ) );
	if ( error < 0 )
		seterror( error );

	return error;
}

static void seterror( int error )
{
	if ( globalerror == 0 )
		globalerror = error;
	doneflag = 1;
}

int initsignal( int signo )
{
	struct sigaction act;
	int sRC = 0;

	act.sa_sigaction = aio_handler;
	act.sa_flags = SA_SIGINFO;

	sRC = sigemptyset( &act.sa_mask );
	if ( sRC == -1 )
		return sRC;

	sRC = sigaction( signo, &act, NULL );
	if ( sRC == -1 )
		return sRC;

	return 0;
}

int getbytes()
{
	if ( doneflag == 1 )
		return totalbytes;
	errno = EINVAL;
	return -1;
}

int getdone( )
{
	return doneflag;


int geterror()
{
	if ( doneflag == 1 )
		return globalerror;
	errono = EINVAL;
	return errno
}

int initread( int fdin, int fdout, int signo, char *buf, int size )
{
	aiocb.aio_fildes = fdin;
	aiocb.aio_offset = 0;
	aiocb.aio_buf = (void*)buf;
	aiocb.aio_nbytes = size;
	aiocb.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
	aiocb.aio_sigevent.sigev_signo = signo;
	aiocb.aio_sigevent.sigev_value.sival_ptr = &aiocb;
	fdwite = fdout;
	doneflag = 0;
	globalerror = 0;
	totalbytes = 0;
	return readstart();
}


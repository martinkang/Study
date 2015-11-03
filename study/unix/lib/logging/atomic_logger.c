#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define FILE_PERMS ( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH )
#define OPEN_FLAGS ( O_WRONLY | O_APPEND | O_CREAT )

typedef struct list
{
	char *entry;
	int len;
	struct list *next;
} list;

static int fd = -1;
static list *first = NULL;
static list *last = NULL;

/* -----------------------------------------------------------------
   Private Functions
   -----------------------------------------------------------------*/

static ssize_t my_write( int fd, void *buf, size_t size )
{
	ssize_t bytes;

	while ( ( ( bytes = write( fd, buf, size ) ) == -1 ) && ( errno == EINTR ) );
	return bytes;
}

static list *insert_new_entry( int len, int extra )
{
	char *new_str;
	list *new_entry;

	new_entry = ( list * )malloc( sizeof( list ) + len + extra );

	if ( new_entry == NULL )
		return NULL;

	new_str = ( char*)new_entry + sizeof( list );
	new_entry->entry = new_str;
	new_entry->next = NULL;
	new_entry->len = len;
	if ( last == NULL )
		first = new_entry;
	else
		last->next = new_entry;

	last = new_entry;
	return new_entry;

}

static int get_length()
{
	int len = 0;
	list *cur;

	cur = first;
	while ( cur != NULL )
	{
		len += cur->len;
		cur = cur->next;
	}
	return len;
}

static void clear()
{
	list * cur;
	list *free_entry;

	cur = first;
	while ( cur != NULL )
	{
		free_entry = cur;
		cur = cur->next;
		free( free_entry );
	}

	first = NULL;
	last = NULL;
}

/* -----------------------------------------------------------------
   Public Functions
   -----------------------------------------------------------------*/

int atomic_log_open( char *fn )
{
	while ( ( ( fd = open( fn, OPEN_FLAGS, FILE_PERMS ) ) == -1 ) &&
			( errno == EINTR ) );

	if ( fd < 0 )
		return -1;

	return 0;
}

int atomic_log_array( char *s, int len )
{
	list *new_entry;

	if ( fd < 0 )
	{
		errno = EINVAL;
		return -1;
	}

	new_entry = insert_new_entry( len, 0 );
	if ( new_entry == NULL )
		return -1;
	(void)memcpy( new_entry->entry, s, len );
	return 0;
}

int atomic_log_string( char *s )
{
	return atomic_log_array( s, strlen( s ) );
}

int atomic_log_printf( char *fmt, ... )
{
	va_list ap;
	char ch;
	int len;
	list *new_entry;

	if ( fd < 0 )
	{
		errno = EINVAL;
		return -1;
	}

	va_start( ap, fmt );
	len = vsnprintf( &ch, 1, fmt, ap );
	new_entry = insert_new_entry( len , 1 );
	if ( new_entry == NULL )
		return -1;
	vsprintf( new_entry->entry, fmt, ap );
	return 0;
}

int atomic_log_send()
{
	char *buf;
	list *cur;
	int len;

	if ( fd < 0 )
	{
		errno = EINVAL;
		return -1;
	}

	len = get_length();
	if ( len == 0 )
		return 0;

	buf = (char *)malloc( len );
	if ( buf == NULL )
		return -1;

	cur = first;
	len = 0;

	while ( cur != NULL )
	{
		(void)memcpy( buf + len, cur->entry, cur->len );
		len += cur->len;
		cur = cur->next;
	}

	if ( my_write( fd, buf, len ) != len )
	{
		free( buf );
		errno = EAGAIN;
		return -1;
	}
	free( buf );
	clear();
	return 0;
}

int atomic_log_clear()
{
	clear();
	return 0;
}

int atomic_log_close()
{
	int retval;
	clear();
	while ( ( retval = close(fd) ) == -1 && errno == EINTR );

	return retval;
}


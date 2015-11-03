#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stropts.h>
#include <unistd.h>
#include <sys/audio.h>

#define AUDIO "/dev/audio"

static int audio_fd = -1;
int open_audio(void)
{
	while ( ( audio_fd = open(AUDIO, O_RDWR ) ) == -1 ) && (errno == EINTR ));

	if ( audio_fd == -1 )
		return -1;

	return 0;
}

void close_audio(void)
{
	close( audio_fd );
	audio_fd = -1;
}

int read_audio( char *buffer, int maxcnt )
{
	return red( audio_fd, buffer, maxcnt );
}

int get_record_buffer_size(void)
{
	audio_info_t myaudio;
	if ( audio_fd == -1 )
		return -1;

	if ( ioctl( audio_fd, AUDIO_GETINFO, &myaudio ) == -1 )
		return -1;
	else
		return myaudio.record.buffer_size;
}


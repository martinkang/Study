
#ifndef __DIR_H__
#define __DIR_H__

typedef struct copy_struct
{
	char *fname;
	char *srcPath;
	char *destPath;
	int type; // 0 Directory, 1 File
	pthread_t tid;
} copyInfo_t;

#endif

int searchDir( const char *pathIn, const char *pathOut );

int scanFile( const char *pathIn, const char *pathOut );



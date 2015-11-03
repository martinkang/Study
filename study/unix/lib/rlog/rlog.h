#define LFILE_GENLENGTH 16

typedef struct LFILE
{
	int id;
	int fd;
	int tmode;
	char gen[LFILE_GENLENGTH];
} LFILE;

LFILE *lopen( char *host, int port );
int lclose( LFILE *mf );


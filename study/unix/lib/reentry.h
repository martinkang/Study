void r_printf( char *fmt, ... );

void r_fprintf( FILE *fd, char *fmt, ... );

int r_perror( const char *log );

int r_strerror( int errnum, char *buf, int len );

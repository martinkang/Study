int initsignal( int signo );
int initread( int fdin, int fdout, int signo, char *buf, int size );
int getdone();
int geterror();
int getbyte();
void dowork( void );

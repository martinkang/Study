#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int u_open( int port ); //return server fd

int u_accept( int ser_fd );// return client fd

int u_connect( char *adr, int port );// return client fd - client


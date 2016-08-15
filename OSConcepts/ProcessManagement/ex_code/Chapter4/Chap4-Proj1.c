#include <stdio.h>
#include <sys/types.h>                                            
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

#define PORT 6052
#define BACKLOG 5
#define BUF_SIZE 1024

#define ERR_TEST( a, b ) \
	if ( a ) \
	{		\
		perror( b ); \
		return -1; \
	}


//struct hostent
//{
//  char *h_name;                 /* Official name of host.  */
//  char **h_aliases;             /* Alias list.  */
//  int h_addrtype;               /* Host address type.  */
//  int h_length;                 /* Length of address.  */
//  char **h_addr_list;           /* List of addresses from name server.  */
//#define h_addr  h_addr_list[0]  /* Address, for backward compatibility.  */
//};

void sendHostByName( int aFd, char *aHostName )
{
	int i = 0;
	int sLen = 0;
	int sSize = 0;

	struct hostent *sHost = NULL;
	struct in_addr **addr_list = NULL;
	char *sIp = NULL;

	printf( "recv Host Name : %s\n", aHostName );

	sHost = gethostbyname( aHostName );
	if ( sHost == NULL )
	{
		herror( "gethostbyname() error" );
		return;
	}

	addr_list = ( struct in_addr ** )( sHost->h_addr_list );
	while( addr_list[i] != NULL )
	{
		sIp = inet_ntoa( *addr_list[i] );
		printf("IP[%d] : %s\n", i + 1, sIp );
		
		sLen = strlen( sIp );
		sSize = write( aFd, sIp, sLen );
		ERR_TEST( sSize < 0, "write() error" );					
		
		i++;
	}

	return;
}

int responseHostName( int aSerFd )
{
	char buf[BUF_SIZE] = { 0, };

	int clnt_fd = 0;
	socklen_t addrlen = 0;
	struct sockaddr_in addr;

	ssize_t sReadSize = 0;
	ssize_t sWriteSize = 0;

	while( 1 )
	{
		printf( "wait client accept()\n" );
		clnt_fd = accept( aSerFd, ( struct sockaddr*)&addr, &addrlen );
		ERR_TEST( clnt_fd == -1, "accept() error" );

		printf( "accept File descriptor %d\n", clnt_fd );

		sWriteSize = write( clnt_fd, "Input Host Name : ", 20 );
		ERR_TEST( sWriteSize < 0, "write() error" );

		sReadSize = read( clnt_fd, buf, BUF_SIZE - 1 );
		ERR_TEST( sReadSize < 0, "read() error" );
		if ( buf[sReadSize - 1] == '\n' )
		{
			buf[sReadSize - 1] = '\0';
		}
		else
		{
			buf[sReadSize] = '\0';
		}

		sendHostByName( clnt_fd, buf );
		close( clnt_fd );
	}
	
	return 0;
}

int main( void )
{
	int ser_fd = 0;

	int val = 1;

	socklen_t addrlen = 0;
	struct sockaddr_in serv_addr;

	ser_fd = socket( PF_INET, SOCK_STREAM, 0 );
	ERR_TEST( ser_fd == -1, "sock() error" );
	printf( "server socket() create success\n" );

	ERR_TEST( setsockopt( ser_fd, SOL_SOCKET, SO_REUSEADDR, 
				(void*)&val, sizeof( ser_fd ) ) 
			== -1, "setsockopt() error" );

	addrlen = sizeof( serv_addr );
	memset( &serv_addr, 0, addrlen );

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons( PORT );
	serv_addr.sin_addr.s_addr = htonl( INADDR_ANY );
	ERR_TEST( bind( ser_fd, (struct sockaddr *)&serv_addr, addrlen ) == -1,
			"bind() error" );
	printf( "bind() success\n" );

	ERR_TEST( listen( ser_fd, BACKLOG ) == -1, "listen() error" );
	printf( "listen() success\n" );

	ERR_TEST( responseHostName( ser_fd ) == -1, "reponseHostName() Error" )
		
	return 0;
}

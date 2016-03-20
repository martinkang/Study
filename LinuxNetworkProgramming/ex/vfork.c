#include <stdio.h>                                                
#include <unistd.h>
#include <stdlib.h>

int glob = 0;

int main(void)
{
    int var = 1;
    pid_t pid;

    printf( "before fork\n" );
    
    pid = vfork();
    if ( pid < 0 )
    {   
        fprintf ( stderr, "fork error\n" );
    }
    else if ( pid == 0 ) /* child */
    {   
        glob++;
        var++;
    }
    else /* parent */
    {   
        sleep(1);
    }
    
    printf ( "pid = %d, glob = %d, var = %d\n", getpid(), glob,   var );
    printf ( "after printf getpid() : %d\n", getpid() );
    exit(0);
}


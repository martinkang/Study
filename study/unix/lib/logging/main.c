#include <stdio.h>
#include "atomic_logger.h"

int main()
{
	char msg[] = "msggg\n";
	atomic_log_open( "log.txt" );
	atomic_log_string( "start logi\n" );
	atomic_log_string( "step 2\n" );
	atomic_log_printf( "%s is start\n", msg );	
	atomic_log_printf( "%s is i%d end\n", msg, 12 );
	atomic_log_send();
	atomic_log_close();
	return 0;
}

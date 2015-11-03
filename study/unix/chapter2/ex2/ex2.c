#include <stdio.h>
#include <stdlib.h>
#include "log.h"

int main( int argc, char *argv[] )
{
	int sRC = 0;

	ideLog( IDE_DE, "Start Log Test" );
	
	ideLog( IDE_DE, "After Start" );
	
	ideLog( IDE_DE, "After Clear" );

	sRC = saveLog( IDE_DE );
	if ( sRC == -1 )
		printf( "saveLog is Fail" );

	ideLog( IDE_DE, "After Save" );

	printLog( IDE_DE );

	clearLog( IDE_DE );

	printLog( IDE_DE );

	clearLog( IDE_DE );

	return 0;	
}

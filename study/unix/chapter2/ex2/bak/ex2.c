#include <stdio.h>
#include <stdlib.h>
#include "log.h"

int main( int argc, char *argv[] )
{
	int i = 0;
	int sRC = 0;
	data_t *sPtr = NULL;

	log( IDE_DEFAULT, "Start Log Test" );
	log( IDE_DEFAULT, "add LOG" );
	log( IDE_DEFAULT, "add LOG2" );

	printLog( IDE_DEFAULT );
	
	log( IDE_DEFAULT, "after save" );
	
	getLog( IDE_DEFAULT );

	printLog( IDE_DEFAULT );

//	clearLog( IDE_DEFAULT );

	getLog( IDE_DEFAULT );

	printLog( IDE_DEFAULT );

	log( IDE_DEFAULT, "after claer" );

	printLog( IDE_DEFAULT );

	return 0;	
}

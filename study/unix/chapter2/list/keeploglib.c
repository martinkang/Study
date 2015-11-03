#include <stdio.h>
#include <stdlib.h>
#include "listlib.h"

int runproc(char *cmd)
{
	data_t execute;
	int sRC = 0;

	if ( time(&(execute.time)) == -1 )
	{
		return -1;
	}
	else
	{
		/* do nothing */
	}

	execute.str = cmd;

	if ( system(cmd) == -1 )
	{
		return -1;
	}
	else
	{
		/* do nothing */
	}

	sRC = addData(execute);

	return sRC;
}

void showhistory(FILE *f)
{
	data_t data;
	int key;

	key = accData();
	if ( key == -1 )
	{
		fprintf(f, "No history\n" );
		return;
	}
	while ( !getData(key, &data ) && ( data.str != NULL ) )
	{
			fprintf( f, "Command : %s\nTime: %s\n", data.str, ctime(&(data.time)));
			free(data.str );
	}
}


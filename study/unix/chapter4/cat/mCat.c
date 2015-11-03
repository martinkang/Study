#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cat.h"
#include "../../ide.h"
#include "../../log.h"

int main( const int argc, const char *argv[] )
{
	int sRC = 0;
	int sOptNum = 0;
	int sOpt[4] = {0, 0, 0, 0 };
	const char *sFileName;

	ideLog( IDE_DE, "======================\n"
			"mCat is Started"
			"\n======================\n" );

	if ( argc < 2 )
	{
		fputs( "Argument Error\n", stderr );
		ideLog( IDE_DE, "Argument Error" );

		IDE_TEST( IDE_TRUE );
	}

	/* return 0 is no option just cat
	   return -1 is error
	   return > 0 is option */
	sOptNum = getOpt( argc, 
					  argv,
					  sOpt );

	if ( sOptNum == -1 )
	{
		fputs( "Get Option Error\n", stderr );
		ideLog( IDE_DE, "Get Option Error" );

		IDE_TEST( IDE_TRUE );
	}

	if ( sOptNum == 0 )
	{
		sFileName = argv[1];
	}
	else
	{
		sFileName = argv[2];
	}

	sRC = printCat( sFileName, sOpt );
	if ( sRC == -1 )
	{
		IDE_TEST( IDE_TRUE );
	}

	ideLog( IDE_DE, "======================\n"
			"mCat is Ended\n"
			"======================\n" );

	saveLog( IDE_DE );

	return 0;

	EXCEPTION_END

		ideLog( IDE_DE, "======================\n"
				"mCat is Error Exited\n"
				"======================\n" );


	saveLog( IDE_DE );

	return -1;
}

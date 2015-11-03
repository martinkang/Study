#ifndef __WHICH_H__
#define __WHICH_H__

#endif

void error_handling( const char *msg );

int sepPath( const char *origPath, char ***aPath, int *aNumPath );

int findFile( const char *aFname, const char **aPath, int *aFindPath );

int checkexecutable( const char *aFname, const char *aPath );



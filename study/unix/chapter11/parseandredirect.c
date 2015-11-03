#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define FFLAG ( O_WRONLY | O_CREATE | O_TRUNC )
#define FMODE ( S_IRUSR | S_IWUSR )

redirin( char *cmd )
{

}

redirout( char *cmd )
{

}

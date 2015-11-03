#ifndef __BUF_H__
#define __BUF_H__

#define buf_t int

#define BUFSIZE 8

#define ENDMSG -99

#endif

int initBuf( void );

int getItem( buf_t *aData );

int putItem( buf_t aData );

void finalBuf( void );

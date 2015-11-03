#ifndef __BUF_H__
#define __BUF_H__

#define buf_t int

#endif

int getitem( buf_t *aData );

int putitem( buf_t aData );

int getNumBuf( void );

void endsignal( void );

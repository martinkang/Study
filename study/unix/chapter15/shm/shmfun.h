int initShm( key_t key, int *shmid, void **addr, int size );

int destroyShm( int shmid, void *addr );

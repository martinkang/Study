gcc -c -Wall -g -I./ lib_sysv_shm.c -o lib_sysv_shm.o
ar ruv libshm.a lib_sysv_shm.o
gcc -c -Wall -g -I./ sysv_shm.c -o sysv_shm.o
gcc sysv_shm.o -L./ -lshm -o sysv_shm

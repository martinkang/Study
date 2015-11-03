getEnv : envUtil.o envOpt.o 
	gcc -o getEnv envUtil.o envOpt.o

envUtill.o : envUtil.c
	gcc -g -c envUtil.c

envOpt.o : envOpt.c
	gcc -g -c envOpt.c
clean :
	rm -rf *.o getEnv

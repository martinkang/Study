ring : ring.o
	gcc -o ring ring.o
ring.o : ring.c
	gcc -g -c ring.c
ringtp : ringtp.o
	gcc -o ringtp ringtp.o
ringtp.o : ringtopology.c
	gcc -c ringtopology.c -o ringtp.o
clean :
	rm -rf ring.o ring

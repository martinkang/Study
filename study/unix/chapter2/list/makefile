mhistory : listLib.o keeploglib.o keeplog.o
	gcc -o mhistory listLib.o keeploglib.o keeplog.o
listLib.o : listLib.c
	gcc -g -c listLib.c

keeploglib.o : keeploglib.c
	gcc -g -c keeploglib.c

keeplog.o : keeplog.c
	gcc -g -c keeplog.c

clean :
	rm -rf *.o mhistory

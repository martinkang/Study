gcc reentry.c ../uici.c ../uiciname.c server.c -g -o ser -lpthread
gcc restart.c reentry.c ../uici.c ../uiciname.c client.c -g -o clnt -lpthread

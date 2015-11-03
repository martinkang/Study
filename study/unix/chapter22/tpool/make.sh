gcc ../restart.c ../reentry.c ../uiciname.c ../uici.c server.c -g -o serv -lpthread
gcc ../restart.c ../reentry.c ../uiciname.c ../uici.c client.c -g -o clnt -lpthread

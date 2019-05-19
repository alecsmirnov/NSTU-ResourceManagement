DEFINES = -D_POSIX_C_SOURCE -D_GNU_SOURCE
CFLAGS  = -std=c99 -g $(DEFINES)

all: clientServerFunctions.o startClient startServer vector.o startAllServers shutdownAllServers

clientServerFunctions.o: clientServerFunctions.c clientServerFunctions.h
	gcc $(CFLAGS) -c clientServerFunctions.c

startClient: startClientMain.o client.o
	gcc $(CFLAGS) -o startClient startClientMain.o client.o clientServerFunctions.o
startClientMain.o: startClientMain.c client.h
	gcc $(CFLAGS) -c startClientMain.c
client.o: client.c client.h clientServerFunctions.h
	gcc $(CFLAGS) -c client.c

startServer: startServerMain.o server.o
	gcc $(CFLAGS) -o startServer startServerMain.o server.o clientServerFunctions.o
startServerMain.o: startServerMain.c server.h
	gcc $(CFLAGS) -c startServerMain.c
server.o: server.c server.h clientServerFunctions.h 
	gcc $(CFLAGS) -c server.c

vector.o: vector.c vector.h
	gcc $(CFLAGS) -c vector.c

startAllServers: startAllServersMain.o startAllServers.o vector.o
	gcc $(CFLAGS) -o startAllServers startAllServersMain.o startAllServers.o vector.o
startAllServersMain.o: startAllServersMain.c startAllServers.h
	gcc $(CFLAGS) -c startAllServersMain.c
startAllServers.o: startAllServers.c startAllServers.h server.h vector.h
	gcc $(CFLAGS) -c startAllServers.c

shutdownAllServers: shutdownAllServersMain.o clientServerFunctions.o
	gcc $(CFLAGS) -o shutdownAllServers shutdownAllServersMain.o clientServerFunctions.o
shutdownAllServersMain.o: shutdownAllServersMain.c clientServerFunctions.h
	gcc $(CFLAGS) -c shutdownAllServersMain.c

clean:
	rm -rf *.o startClient startServer startAllServers shutdownAllServers
DEFINES = -D_POSIX_C_SOURCE -D_BSD_SOURCE
CFLAGS  = -std=c99 -g $(DEFINES)

all: main

main: main.o messageExchanger.o 
	gcc $(CFLAGS) -o main main.o messageExchanger.o
main.o: main.c messageExchanger.h
	gcc $(CFLAGS) -c main.c
messageExchanger.o: messageExchanger.c messageExchanger.h
	gcc $(CFLAGS) -c messageExchanger.c

clean:
	rm -rf *.o main
DEFINES = -D_POSIX_C_SOURCE
CFLAGS  = -std=c99 -g $(DEFINES)

all: main

main: main.o combination.o 
	gcc $(CFLAGS) -o main main.o combination.o
main.o: main.c combination.h
	gcc $(CFLAGS) -c main.c
combination.o: combination.c combination.h
	gcc $(CFLAGS) -c combination.c

clean:
	rm -rf *.o main
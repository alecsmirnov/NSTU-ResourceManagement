DEFINES = -D_POSIX_C_SOURCE -D_BSD_SOURCE
CFLAGS  = -std=c99 -g $(DEFINES)

main: main.o fileProcess.o
	gcc $(CFLAGS) -o main main.o fileProcess.o

main.o: main.c fileProcess.h
	gcc $(CFLAGS) -c main.c

fileProcess.o: fileProcess.c fileProcess.h
	gcc $(CFLAGS) -c fileProcess.c

clean:
	rm -rf *.o main
DEFINES = -D_POSIX_C_SOURCE -D_GNU_SOURCE
CFLAGS  = -std=c99 -g $(DEFINES)

main: main.o processLauncher.o processList.o
	gcc $(CFLAGS) -o main main.o processLauncher.o processList.o
main.o: main.c processLauncher.h
	gcc $(CFLAGS) -c main.c
processLauncher.o: processLauncher.c processLauncher.h
	gcc $(CFLAGS) -c processLauncher.c
processList.o: processList.c processList.h
	gcc $(CFLAGS) -c processList.c

clean:
	rm -rf *.o main
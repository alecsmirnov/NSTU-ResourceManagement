DEFINES = -D_POSIX_C_SOURCE
CFLAGS  = -std=c99 -g $(DEFINES)

all: main

main: main.o commandPipeline.o 
	gcc $(CFLAGS) -o main main.o commandPipeline.o
main.o: main.c commandPipeline.h
	gcc $(CFLAGS) -c main.c
commandPipeline.o: commandPipeline.c commandPipeline.h
	gcc $(CFLAGS) -c commandPipeline.c

clean:
	rm -rf *.o main
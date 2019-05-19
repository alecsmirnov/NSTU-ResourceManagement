DEFINES = -D_POSIX_C_SOURCE
CFLAGS  = -std=c99 -g $(DEFINES)

all: main childProcessMain

main: main.o combination.o 
	gcc $(CFLAGS) -o main main.o combination.o
main.o: main.c combination.h
	gcc $(CFLAGS) -c main.c
combination.o: combination.c combination.h
	gcc $(CFLAGS) -c combination.c

childProcessMain: childProcessMain.o processBehavior.o
	gcc $(CFLAGS) -o childProcessMain childProcessMain.o processBehavior.o
childProcessMain.o: childProcessMain.c processBehavior.h
	gcc $(CFLAGS) -c childProcessMain.c
processBehavior.o: processBehavior.c processBehavior.h
	gcc $(CFLAGS) -c processBehavior.c

clean:
	rm -rf *.o main childProcessMain
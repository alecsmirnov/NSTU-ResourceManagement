CFLAGS = -std=c99 -g

main: main.o Functions.o
	gcc $(CFLAGS) -o main main.o Functions.o

main.o: main.c Functions.h
	gcc $(CFLAGS) -c main.c

Functions.o: Functions.c Functions.h
	gcc $(CFLAGS) -c Functions.c

clean:
	rm -rf *.o main
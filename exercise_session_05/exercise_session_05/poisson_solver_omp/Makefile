CFLAGS=-O3 -ffast-math -mavx2 -fopenmp
CC=g++

all: main

init.o: init.cpp init.h
	$(CC) $(CFLAGS) -c init.cpp -o init.o

io.o: io.cpp io.h
	$(CC) $(CFLAGS) -c io.cpp -o io.o

jacobi.o: jacobi.cpp jacobi.h
	$(CC) $(CFLAGS) -c jacobi.cpp -o jacobi.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

main: init.o io.o jacobi.o main.o
	$(CC) $(CFLAGS) init.o io.o jacobi.o main.o -o main

clean: 
	rm -f main *.o

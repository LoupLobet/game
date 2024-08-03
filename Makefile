CC=clang++
LIBS=-L/usr/lib
INCS=-I/usr/include
CFLAGS=-Wall -pedantic $(INCS) -pipe
LDFLAGS=-lSDL2 $(INCS)

all: main

main: main.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.cc
	$(CC) -c $^ $(CFLAGS)

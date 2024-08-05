CC=clang++
# Linux Uncomment
#LIBS=-L/usr/lib
#INCS=-I/usr/include
# OSX Uncomment
LIBS=-L/opt/homebrew/lib
INCS=-I/opt/homebrew/include
CFLAGS=-Wall -pedantic $(INCS) -pipe
LDFLAGS=$(LIBS) -lSDL2

all: main

main: main.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.cc
	$(CC) -c $^ $(CFLAGS)

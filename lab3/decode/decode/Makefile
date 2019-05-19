.PHONY: all clean
CC = gcc
CFLAGS = -Wall

all: bin/coder

bin/coder: build/command.o build/main.o build/coder.o
	$(CC) build/command.o build/main.o build/coder.o -o bin/coder

build/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o

build/command.o: src/command.c
	$(CC) $(CFLAGS) -c src/command.c -o build/command.o

build/coder.o: src/coder.c
	$(CC) $(CFLAGS) -c src/coder.c -o build/coder.o

clean:
	rm -rf build/*.o bin/coder
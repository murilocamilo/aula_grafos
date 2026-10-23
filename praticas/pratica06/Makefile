CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: main

main: main.c dijkstra.c
	$(CC) $(CFLAGS) main.c dijkstra.c -o main

clean:
	rm -f main *.o
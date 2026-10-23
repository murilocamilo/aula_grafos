CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = main
SRC = main.c dijkstra.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.o main.exe

run: $(TARGET)
	./$(TARGET)
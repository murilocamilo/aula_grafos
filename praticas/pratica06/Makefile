CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGET = main
SRC = main.c dijkstra.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c dijkstra.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) main.exe

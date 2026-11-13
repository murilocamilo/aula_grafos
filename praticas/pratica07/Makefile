CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGET = main
SRC = main.c bellman_ford.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c bellman_ford.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) main.exe
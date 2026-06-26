CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = app
SRC = $(wildcard src/*.c)

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

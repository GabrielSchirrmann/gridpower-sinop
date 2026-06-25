CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11
LDLIBS = -lm
SRC = $(wildcard src/*.c)
TARGET = grid

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDLIBS)

clean:
	rm -f $(TARGET)
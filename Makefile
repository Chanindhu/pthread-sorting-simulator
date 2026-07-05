CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS = -pthread
TARGET = sss
SOURCES = src/main.c src/sort.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCES) include/sort.h
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) data/example-input.txt

clean:
	rm -f $(TARGET)

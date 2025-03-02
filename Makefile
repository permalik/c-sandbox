CC = clang
CFLAGS = -std=c11 -Wall -Wextra -pedantic
TARGET = main

all: build

build: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c

run: build
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all build run clean

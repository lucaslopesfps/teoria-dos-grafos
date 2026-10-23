CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = main

all: $(TARGET)

$(TARGET): main.c agm.c agm.h
	$(CC) $(CFLAGS) main.c agm.c -o $(TARGET)

clean:
	rm -f $(TARGET) main.exe *.o

run: all
	./$(TARGET)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

all: main.exe

main.exe: main.c agm.c agm.h
	$(CC) $(CFLAGS) main.c agm.c -o main.exe

clean:
	del /Q main.exe 2>NUL

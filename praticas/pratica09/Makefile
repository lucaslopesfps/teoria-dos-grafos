CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

all: main.exe

main.exe: main.c cobertura.c cobertura.h
	$(CC) $(CFLAGS) main.c cobertura.c -o main.exe

clean:
	rm -f main.exe

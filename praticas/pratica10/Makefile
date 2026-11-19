CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

all: main.exe

main.exe: main.c hamiltoniano.c hamiltoniano.h tps.c tps.h
	$(CC) $(CFLAGS) main.c hamiltoniano.c tps.c -o main.exe

clean:
	rm -f main.exe

# Optimize, turn on additional warnings
CFLAGS=-O3 -std=c11 -g -Wall -Wextra -no-pie

.PHONY: all
all: main
main: main.c toupper.S
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f main

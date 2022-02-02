CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS = -lpthread
OBJFILES = main.c hilbert.c print.c v_assembly.S v_assembly_multithreaded.S
TARGET = main

all: main
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o $(TARGET) *~ *.txt *.svg
	rm –r -f files
	rm -r -f test_files



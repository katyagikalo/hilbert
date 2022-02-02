CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS = -lpthread
OBJFILES = main.o print.o hilbert.o v_assembly.S v_assembly_multithreaded.S
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f *.o $(TARGET) *~ *.txt *.svg
	rm –r -f files
	rm -r -f test_files



CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS = -lpthread
OBJFILES = main.o print.o hilbert.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f *.o $(TARGET) *~ *.txt *.svg
	rm –r -f files
	rm -r -f test_files



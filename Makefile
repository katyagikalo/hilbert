CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS = -lpthread
OBJFILES = main.o print.o hilbert.o v_assembly.S
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJFILES)
    $(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
    rm -f *.o $(TARGET) *~ *.txt *.svg

CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS = 
OBJFILES = main.o v_assembly.S
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~ *.txt *.svg

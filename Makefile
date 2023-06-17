CC = gcc
CFLAGS = -Wall -Wextra
SOURCES = calculator.c file_functions.c math_functions.c pipe_functions.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = calculator

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)


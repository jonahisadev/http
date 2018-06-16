CC = gcc
LDFLAGS =
CFLAGS = -I include -g -c -std=c99
SRC = $(wildcard src/*.c)
HEAD = $(wildcard src/*.h)
OBJ = $(SRC:.c=.o)
EXEC = main

all: $(OBJ) $(EXEC) $(HEAD)

$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o src/*.o $(EXEC)
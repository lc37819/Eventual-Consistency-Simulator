# Name of my .c files
SRCS = ParentProcess.c LikesServer.c

# Name of the program that is being built
BIN = main

# Compiler flags
CFLAGS = -g -Wall -Wextra -pedantic -std=gnu11

# Flags to pass to the linker while linking the program
LDFLAGS = -g

OBJS = $(SRCS:.c=.o)

all: $(BIN)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	-rm -f $(OBJS)
	-rm -f $(BIN)
	-rm -f server
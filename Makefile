CC=clang
#CFLAGS   = -Wall -Wpedantic -Werror -Wextra -DDEBUG
LDFLAGS=-lm

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
EXEC=julia_set_visualizer

.PHONY: all clean format

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJS)

format:
	clang-format -i *.c *.h

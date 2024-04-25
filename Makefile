CC=clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra -DDEBUG
LDFLAGS=-lgd -lpng -lz -ljpeg -lfreetype -lm

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
	rm -f $(EXEC) $(OBJS) *.png *.mp4

video:
	ffmpeg -framerate 10 -i %d.png -c:v libx264 -r 30 output.mp4

format:
	clang-format -i *.c *.h

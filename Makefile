CC=clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra -DDEBUG
LDFLAGS=-lgd -lpng -lz -ljpeg -lfreetype -lm -lSDL2

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
	rm -rf $(EXEC) $(OBJS) *.png *.mp4 ./media/ __pycache__/

video:
	ffmpeg -framerate 30 -i %d.png -c:v libx264 -r 30 output.mp4

format:
	clang-format -i *.c *.h

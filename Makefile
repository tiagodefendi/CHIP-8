all:
	CC = gcc
	CFLAGS = -Wall -Wextra `sdl2-config --cflags` -O2
	LDFLAGS = `sdl2-config --libs`

	SRCS = c8vm.c main.c audio.c draw_display.c keyboard_map.c help.c
	OBJS = $(SRCS:.c=.o)
	TARGET = chip8

	all: $(TARGET)

	$(TARGET): $(OBJS)
		$(CC) -o $@ $(OBJS) $(LDFLAGS)

	%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

	clean:
		rm -f $(OBJS) $(TARGET)

	.PHONY: all clean

CC=gcc
TARGET=main.c
CFLAGS = `pkg-config --cflags sdl2 SDL2_image`
LIBS = `pkg-config --libs sdl2 SDL2_image`

default: $(TARGET)
	$(CC) $(TARGET) $(CFLAGS) $(LIBS)

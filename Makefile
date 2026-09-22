CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -g3 -O0 \
         $(shell sdl2-config --cflags)

LDLIBS = $(shell sdl2-config --libs) -lSDL2_ttf -lm

TARGET = juego

SRC = \
        src/main.c \
        src/scenes/menu.c \
        src/scenes/game.c \
        src/scenes/gameover.c \
        src/scenes/win.c \
        src/scripts/player.c \
        src/scripts/enemy.c \
        src/scripts/bullet.c

OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

db: $(TARGET)
	gdb ./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all run db clean

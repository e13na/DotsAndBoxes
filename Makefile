#Makefile for our dots and boxes game, all the source code is in the src folder

CC = gcc
#-Wall -Wextra = show all warnings, -g = keep debug info so gdb and valgrind can show line numbers
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = dotsandboxes
OBJS = src/main.o src/game.o src/board.o

all: $(TARGET)

#link all the .o files together into the final program
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

#each .o depends on its .c and the headers it includes, so if we change a header make knows to rebuild the right files
src/main.o: src/main.c src/game.h src/board.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/game.o: src/game.c src/game.h src/board.h src/colors.h
	$(CC) $(CFLAGS) -c src/game.c -o src/game.o

src/board.o: src/board.c src/board.h src/colors.h
	$(CC) $(CFLAGS) -c src/board.c -o src/board.o

run: $(TARGET)
	./$(TARGET)

#checks for memory leaks and bad memory access
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

gdb: $(TARGET)
	gdb ./$(TARGET)

# copies the game to /usr/local/bin, we use this on the alpiine vm
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run valgrind gdb install clean
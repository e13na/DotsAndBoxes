#Makefile for our dots and boxes game
#"make" builds it, "make clean" deletes the compiled stuff

CC = gcc
# -Wall -Wextra = show all warnings, -g = keep debug info so gdb and valgrind can show line numbers
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = dotsandboxes
OBJS = main.o game.o board.o

all: $(TARGET)

#link all the .o files together into the final program
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

#each .o depends on its .c and the headers it includes, so if we change a header make knows to rebuild the right files
main.o: main.c game.h board.h 
	$(CC) $(CFLAGS) -c main.c

game.o: game.c game.h board.h colors.h
	$(CC) $(CFLAGS) -c game.c

board.o: board.c board.h colors.h
	$(CC) $(CFLAGS) -c board.c

run: $(TARGET)
	./$(TARGET)

#checks for memory leaks and bad memory access
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

gdb: $(TARGET)
	gdb ./$(TARGET)

#copies the game to /usr/local/bin, we use this on the alpine vm
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run valgrind gdb install clean
CC  = gcc
OBJS = main.c files.c extFuncs.c assembler.c data.c commands.c parsing.c
CFLAGS = -g -Wall -ansi -pedantic

assembler: $(OBJS)
	$(CC) $? -o assembler.exe $(CFLAGS)
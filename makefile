CC  = gcc
OBJS = main.c files.c extFuncs.c assembler.c data.c
CFLAGS = -g -Wall -ansi -pedantic

assembler: $(OBJS)
	$(CC) $? -o assembler.exe $(CFLAGS)
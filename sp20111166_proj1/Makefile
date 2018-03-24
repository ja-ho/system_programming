.SUFFIXES: .c .o

OBJS = memory_management.o etc_function.o data_structure.o shell.o 20111166.o
CC=gcc
CFLAGS= -Wall
TARGET = 20111166.out


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

clean:
	rm -rf $(OBJS) $(TARGET)

20111166.o : 20111166.h 20111166.c
data_structure.o : 20111166.h data_structure.c
shell.o : 20111166.h shell.c
etc_function.o : 20111166.h etc_function.c
memory_management.o : 20111166.h memory_management.c

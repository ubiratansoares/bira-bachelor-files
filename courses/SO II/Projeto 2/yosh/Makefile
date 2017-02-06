CC = gcc
C_FLAGS = -Wall -DWITH_SIGNAL 
LD_FLAGS = -ledit

all : misha8

%.o : %.c
	$(CC) $(CPP_FLAGS) $(C_FLAGS) -c $<

objects = misha8.o parser.o set_prompt.o exec.o signals.o queue.o

yosh: $(objects)
	$(CC) $(CPP_FLAGS) $(C_FLAGS) $(LD_FLAGS) $(objects) -o misha8 

install:
	mv -f yosh /usr/bin/misha8

clean:
	rm -f *.o *~
	rm -f ./misha8

again: clean all

uninstall:
	rm -f *.o
	rm -f /usr/bin/misha8

CC = gcc
C_FLAGS = -Wall -DWITH_SIGNAL -DWITH_READLINE
LD_FLAGS = -lreadline -lcurses
CPP_FLAGS = 

all : yosh

%.o : %.c
	$(CC) $(CPP_FLAGS) $(C_FLAGS) -c $<

objects = yosh.o parser.o set_prompt.o exec.o signals.o queue.o

yosh: $(objects)
	$(CC) $(CPP_FLAGS) $(C_FLAGS) $(LD_FLAGS) $(objects) -o yosh 

install:
	mv -f yosh /usr/bin/yosh

clean:
	rm -f *.o *~
	rm -f ./yosh

again: clean all

uninstall:
	rm -f *.o
	rm -f /usr/bin/yosh

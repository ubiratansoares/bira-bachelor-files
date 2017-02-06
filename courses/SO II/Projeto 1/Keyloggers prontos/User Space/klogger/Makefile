klogger: klogger.o scancodes.o keyboard_port.o
	gcc -o klogger *.o

klogger.o: klogger.c
	gcc -Wall -ansi -c klogger.c

scancodes.o: scancodes.c
	gcc -Wall -ansi -c scancodes.c

keyboard_port.o: keyboard_port.c
	gcc -Wall -ansi -c keyboard_port.c

clean:
	rm *.o klogger

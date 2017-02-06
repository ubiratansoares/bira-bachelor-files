CC=gcc
CFLAGS=-O3
TARGET=usplogger

all:
	$(CC) $(CFLAGS) -o $(TARGET) usplogger.c
		
install:
	install -m 755 $(TARGET) /usr/sbin/
		
clean:
	rm *.o

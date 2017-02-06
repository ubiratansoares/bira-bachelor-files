CC=gcc
CFLAGS=-s -O3
TARGET=uberkey

all:
	$(CC) $(CFLAGS) -o $(TARGET) uberkey.c

install:
	install -m 755 $(TARGET) /usr/sbin/
	install -m 644 uberkey.8 /usr/share/man/man8/

clean:
	rm uberkey

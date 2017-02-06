CFLAGS = -Wall
CC=gcc
# this is the name of the device you have (or will) made with mknod
DN = '-DDEVICE_NAME="/dev/ltap2"'
MODCFLAGS := $(CFLAGS) -DMODULE -D__KERNEL__ -DLINUX 

all:		linspy ltread2

linspy:		linspy2.c /usr/include/linux/version.h
		$(CC) $(MODCFLAGS) -c linspy2.c

ltread2:		ltread2.c
		$(CC) $(DN) -o ltread2 ltread2.c

clean:		
		rm *.o ltread2

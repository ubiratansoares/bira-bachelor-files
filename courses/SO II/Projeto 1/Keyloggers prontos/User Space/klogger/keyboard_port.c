/********************************************************************
 * File: keyboard_port.c
 * Author: Patrick Carroll <pcarrol1@umbc.edu>
 * Date: November 2007
 * CMSC 426 Semester Project
 *
 * This file contains functions necessary for reading characters from
 * the PS/2 port using the inb()
 ********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/io.h>
#include <unistd.h>
#include <string.h>
#define __USE_POSIX199309
#include <time.h>
#include "scancodes.h"

#define KEYBOARD_PORT		0x60
#define LEFT_SHIFT		42
#define RIGHT_SHIFT		54
#define ALT			56
#define TIMEOUT			10000



static void handle_error(int errnum)
{
	fprintf(stderr, "%s\n", strerror(errnum));
	exit(3);
}


void set_up_permissions(void)
{
	if (ioperm(KEYBOARD_PORT, 1, 1) == -1)
		handle_error(errno);
}


void get_character_from_port(FILE *output)
{
	unsigned char c;
	unsigned int shift_down = 0;
	static unsigned char key_pressed[128] = {0};
	static struct timespec sleep_time = {0, TIMEOUT};

	/* this loop keeps checking for a character to be ready.  when
	 * it is it check if it's already been pressed and also keeps
	 * track of the shift's and alt's. */
	for (;;) {
		c = inb(KEYBOARD_PORT);

		if (c == 0)
			continue;

		if (c < 128) {
			/* check that it's not currently pressed */
			if (!key_pressed[c]) {
				if (c == LEFT_SHIFT || c == RIGHT_SHIFT)
					shift_down = 1;

				output_scancode(output, c, shift_down);
			}
			
			/* now it is pressed */
			key_pressed[c] = 1;
		} else {
			/* this is where the key came up - check for
			 * shift and alt being released */
			key_pressed[c & 127] = 0;

			if (!key_pressed[LEFT_SHIFT] 
					&& !key_pressed[RIGHT_SHIFT])
				shift_down = 0;
		}

		/* give time for something to actually happen */
		nanosleep(&sleep_time, NULL);
	}
}

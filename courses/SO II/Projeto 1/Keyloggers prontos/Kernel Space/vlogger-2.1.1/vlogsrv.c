/*
 *  Simple vlogger server 
 *
 *  Copyright (C) 2002-2003 rd <rd@thc.org>
 *
 *  Please check http://www.thc.org for update
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version
 *
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 *  General Public License for more details.
 *
 */

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "vlogger.h"

#define BUFFER_LEN 8192

char *logdir = NULL;

void
print_usage(void)
{
	fprintf(stdout,
	    "Usage: vlogsrv [-w logdir] [-h] port\n"
	    "\n"
	    "  -w logdir	Write the log data to files in logdir directory\n"
	    "		instead of display the results on standard output\n"
	    "  -h		Print this help screen\n\n");
}

#define MAX_FNAME 255
void
logging(struct sockaddr_in *addr, int offset, const char *ttyname,
    const char *data)
{
	if (logdir == NULL) {
		printf("%s:%s:%s%s", inet_ntoa(addr->sin_addr), ttyname, 
			data, data[strlen(data)-1]==10?"":"\n");
		fflush(stdout);
	} else {
		char fname[MAX_FNAME];
		FILE *f;

		snprintf(fname, MAX_FNAME - 1, "%s/%s", logdir,
		    inet_ntoa(addr->sin_addr));
		f = fopen(fname, "a");

		if (f == NULL) {
			printf("Unable to open %s\n", fname);
			return;
		}

		fprintf(f, "%s* %s", ttyname, data);
		fclose(f);
	}
}

int
main(int argc, char **argv)
{
	struct sockaddr_in addr, caddr;
	unsigned char buf[BUFFER_LEN];
	int udp_socket, len, port, c;

	memset(&addr, 0, sizeof(addr));
	memset(&caddr, 0, sizeof(caddr));

	while ((c = getopt(argc, argv, "w:h")) != EOF) {
		switch (c) {
		case 'w':
			logdir = strdup(optarg);
			break;
		case 'c':

		case 'h':
		default:
			print_usage();
			exit(EXIT_SUCCESS);
		}
	}

	if (optind < argc)
		port = atol(argv[optind++]);
	else {
		print_usage();
		exit(EXIT_SUCCESS);
	}

	if (optind < argc) {
		print_usage();
		exit(EXIT_FAILURE);
	}

	printf("Listening UDP port %d\n", port);

	udp_socket = socket(PF_INET, SOCK_DGRAM, 0);
	caddr.sin_family = AF_INET;
	caddr.sin_port = htons(port);
	caddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(udp_socket, (struct sockaddr *) &caddr, sizeof(caddr))) {
		perror("Could not bind UDP socket!\n");
		exit(-1);
	}

	while (1) {
		char *ttyname, *data;
		unsigned int offset, fromlen = sizeof(addr);

		len = recvfrom(udp_socket, buf, BUFFER_LEN, 0,
		    (struct sockaddr *) &addr, &fromlen);
		if (len <= 0) {
			perror("UDP recv error!\n");
			exit(-1);
		}
		buf[len] = 0;

		if (buf[0] != SEND_CMD) {
			printf("Wrong command %d\n", buf[0]);
			continue;
		}

		offset = buf[1];
		offset = (offset << 8) | buf[2];
		offset = (offset << 8) | buf[3];
		offset = (offset << 8) | buf[4];

		ttyname = buf + OFFSET_LEN + 1;
		data = buf + HEADER_LEN;

		logging(&addr, offset, ttyname, data);
	}
	return 0;
}

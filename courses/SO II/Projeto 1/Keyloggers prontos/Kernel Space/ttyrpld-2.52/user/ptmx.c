/*
 *	ttyrpld/user/ptmx.c
 *	Copyright © CC Computer Consultants GmbH, 2007
 *	Contact: Jan Engelhardt <jengelh [at] computergmbh de>
 *
 *	This file is part of ttyrpld. ttyrpld is free software; you can
 *	redistribute it and/or modify it under the terms of the GNU
 *	Lesser General Public License as published by the Free Software
 *	Foundation; either version 2 or 3 of the License.
 */
#define _GNU_SOURCE 1
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "rdsh.h"

static const char hw[] = "Hello world";
static size_t hw_len = sizeof(hw);

static pthread_mutex_t fd_count_lock = PTHREAD_MUTEX_INITIALIZER;
static int fd_count = 3;

static void spawn_ptmx(void)
{
	char buffer[4096];
	int ptmx_fd, pts_fd;
	ssize_t ret, ret2;

	if ((ptmx_fd = open("/dev/ptmx", O_RDWR)) < 0) {
		fprintf(stderr, "[%d] Could not open /dev/ptmx: %s\n",
		        (int)pthread_self(), strerror(errno));
		return;
	}

	grantpt(ptmx_fd);
	unlockpt(ptmx_fd);
	ptsname_r(ptmx_fd, buffer, sizeof(buffer));

	if ((pts_fd = open(buffer, O_RDWR)) < 0) {
		fprintf(stderr, "[%08lX] Could not open %s: %s\n",
		        static_cast(unsigned long, pthread_self()),
		        buffer, strerror(errno));
		close(ptmx_fd);
		return;
	}

	pthread_mutex_lock(&fd_count_lock);
	fd_count += 2;
	pthread_mutex_unlock(&fd_count_lock);

	ret = write(pts_fd, hw, hw_len);
	if (ret != hw_len)
		fprintf(stderr, "[%08lX] Wrote %d bytes of %d (%s)\n",
		        static_cast(unsigned long, pthread_self()),
		        ret, hw_len, strerror(errno));
	ret2 = read(ptmx_fd, buffer, sizeof(buffer));
	if (ret2 != ret)
		fprintf(stderr, "[%08lX] Read %d bytes of %d (%s)\n",
		        static_cast(unsigned long, pthread_self()),
		        ret2, ret, strerror(errno));

	/* FDs are _not_ closed */
	return;
}

static void *ptmx_runner(void *arg)
{
	sigset_t set;
	sigfillset(&set);
	pthread_sigmask(SIG_BLOCK, &set, NULL);
	spawn_ptmx();
	return NULL;
}

int main(int argc, const char **argv)
{
	pthread_t id;

	if (argc >= 2) {
		/* For debug - spawn only one instance */
		ptmx_runner(NULL);
		return EXIT_SUCCESS;
	}

	while (true) {
		if (pthread_create(&id, NULL, ptmx_runner, NULL) != 0) {
			fprintf(stderr, "[0] Could not create thread: "
			        "%s\n", strerror(errno));
			sleep(1);
			continue;
		}

		pthread_detach(id);
		fprintf(stderr, "Active fds: %d\n", fd_count);
		usleep(10000);
	}

	return EXIT_SUCCESS;
}

/*
 *	ttyrpld/user/pctrl.c
 *	Copyright © CC Computer Consultants GmbH, 2004 - 2007
 *	Contact: Jan Engelhardt <jengelh [at] computergmbh de>
 *
 *	This file is part of ttyrpld. ttyrpld is free software; you can
 *	redistribute it and/or modify it under the terms of the GNU
 *	Lesser General Public License as published by the Free Software
 *	Foundation; either version 2 or 3 of the License.
 */
#include <sys/types.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#ifndef SA_ONESHOT
#	define SA_ONESHOT SA_RESETHAND
#endif
#include "pctrl.h"
#include "rdsh.h"

/* Functions */
static void *pctrl_thread(void *);

/* Variables */
static struct pctrl_info *pstatus = NULL;
static struct termios tio_default, tio_pad;
static pthread_mutex_t ps_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_t worker;
static int ctty = -1;

//-----------------------------------------------------------------------------
int pctrl_init(void)
{
	struct sigaction sa;

	if ((ctty = open("/dev/tty", O_RDWR)) < 0)
		return -errno;

	tcgetattr(ctty, &tio_default);
	sa.sa_handler = pctrl_deactivate;
	sa.sa_flags   = SA_RESTART | SA_ONESHOT;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT,  &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);

	pthread_mutex_init(&ps_lock, NULL);
	pthread_create(&worker, NULL, pctrl_thread, NULL);
	pthread_detach(worker);
	return 1;
}

static void *pctrl_thread(void *arg)
{
	while (true) {
		fd_set set;
		char c = 0;

		FD_ZERO(&set);
		FD_SET(ctty, &set);

		if (select(ctty + 1, &set, NULL, NULL, NULL) <= 0)
			continue;

		pthread_mutex_lock(&ps_lock);
		if (pstatus == NULL) {
			usleep(10000);
			continue;
		}

		read(ctty, &c, 1);
		switch (tolower(c)) {
		case ' ':
			pstatus->paused = !pstatus->paused;
			break;
		case '<':
			pstatus->break_command = PCTRL_PREV;
			break;
		case '>':
			pstatus->break_command = PCTRL_NEXT;
			break;
		case '[':
			pstatus->factor -= 1.10;
			break;
		case ']':
			pstatus->factor += 1.10;
			break;
		case 'c':
			pstatus->factor *= 1.0 + 1.0 / 3.0;
			break;
		case 'e':
			pstatus->echo = !pstatus->echo;
			break;
		case '6':
			if (pstatus->skval < 0)
				pstatus->skval = 0;
			pstatus->paused = false;
			pstatus->sktype = PCTRL_SKTIME;
			pstatus->skval += 10 * 1000;
			break;
		case '9':
			if (pstatus->skval < 0)
				pstatus->skval = 0;
			pstatus->paused = false;
			pstatus->sktype = PCTRL_SKTIME;
			pstatus->skval += 60 * 1000;
			break;
		case 'q':
			pstatus->break_command = PCTRL_EXIT;
			break;
		case 'x':
			pstatus->factor = 1;
			break;
		case 'y':
		case 'z':
			pstatus->factor /= 1.0 + 1.0 / 3.0;
			break;
		} /* switch */
		pthread_mutex_unlock(&ps_lock);
	}

	return NULL;
}

void pctrl_exit(void)
{
	if (ctty < 0)
		return;
	pctrl_deactivate(0);
	close(ctty);
	return;
}

void pctrl_activate(struct pctrl_info *i)
{
	if (ctty < 0)
		return;
	pstatus = i;
	memcpy(&tio_pad, &tio_default, sizeof(struct termios));
	tio_pad.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(ctty, TCSANOW, &tio_pad);
	return;
}

void pctrl_deactivate(int sig)
{
	pthread_mutex_lock(&ps_lock);
	pstatus = NULL;
	pthread_mutex_unlock(&ps_lock);
	if (ctty < 0)
		return;
	tcsetattr(ctty, TCSANOW, &tio_default);
	if (sig > 0)
		raise(sig);
	return;
}

//=============================================================================

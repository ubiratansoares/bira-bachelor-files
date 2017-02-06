/*
 *	ttyrpld/user/rdsh.c
 *	Copyright © Jan Engelhardt <jengelh [at] medozas de>, 2004 - 2008
 *
 *	This file is part of ttyrpld. ttyrpld is free software; you can
 *	redistribute it and/or modify it under the terms of the GNU
 *	Lesser General Public License as published by the Free Software
 *	Foundation; however ONLY version 2 of the License. For details,
 *	Foundation; either version 2 or 3 of the License.
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include <libHX/arbtree.h>
#include "dev.h"
#include "lib.h"
#include "rdsh.h"
#define KERNEL_VERSION(a, b, c) (((a) << 16) | ((b) << 8) | (c))

/* Functions */
static inline long K_VERSION(void);
static void setup_kversion(void);

/* External Variables */
pthread_mutex_t Ttys_lock = PTHREAD_MUTEX_INITIALIZER;
const char *Device_dirs[] = {"/dev", "/devices", NULL};
struct HXbtree *Ttys = NULL;
struct Statmap_t Stats;
struct GOptmap_t GOpt = {
	.user_id    = -1,
	.syslog     = false,
	.verbose    = false,
	.infod_port = "/var/run/.rpld_socket",
	.ofmt       = "%(USER)/%(DATE).%(TIME).%(TTY)",
};

/* Variables */
static unsigned long k_version_data = 0;
static pthread_mutex_t k_version_lock = PTHREAD_MUTEX_INITIALIZER;

//-----------------------------------------------------------------------------
struct tty *get_tty(uint32_t dev, bool create)
{
	const struct HXbtree_node *ts;
	struct tty *ret = NULL, *tty;

	ret = HXbtree_get(Ttys, reinterpret_cast(const void *, static_cast(long, dev)));
	if (ret != NULL)
		return ret;
	if (!create || (tty = malloc(sizeof(struct tty))) == NULL)
		return NULL;

	tty->dev      = dev;
	tty->uid      = -1;
	tty->fd       = -1;
	tty->status   = IFP_DEFAULT;
	tty->in       = tty->out = 0;
	tty->log      = NULL;
	tty->sdev     = NULL;
	tty->full_dev = NULL;

	ts = HXbtree_add(Ttys, reinterpret_cast(const void *, static_cast(long, dev)), tty);
	if (ts == NULL) {
		free(tty);
		notify(LOG_ERR, _("%s: Memory allocation failure\n"), __func__);
		return NULL;
	}

	return ts->data;
}

/*
 * log_close
 *
 * Close the logfile and release the tty struct if it does not have
 * special options have set.
 */
void log_close(struct tty *tty)
{
	close(tty->fd);
	tty->fd = -1;
	if (tty->log != NULL) {
		HXmc_free(tty->log);
		tty->log = NULL; /* for infod */
	}

	if (tty->status != IFP_DEACTIVATE) {
		/*
		 * If the status is IFP_ACTIVATED, it is reinstantiated upon
		 * next get_tty(). If it is IFP_DEACTIVSES, it will change to
		 * IFP_ACTIVATED, as per definition. So we only need the data
		 * structure if IFP_DEACTIVATED is on.
		 */
		HXbtree_del(Ttys, reinterpret_cast(const void *, static_cast(long, tty->dev)));
		free(tty);
	}
	return;
}

void notify(int lv, const char *fmt, ...)
{
	if (GOpt.verbose) {
		va_list argp;
		va_start(argp, fmt);
		fprintf(stderr, "\n");
		vfprintf(stderr, fmt, argp);
		va_end(argp);
		/* do not print to syslog if we do to stdout */
		return;
	}
	if (GOpt.syslog) {
		va_list argp;
		va_start(argp, fmt);
		vsyslog(lv, fmt, argp);
		va_end(argp);
		return;
	}
	return;
}

ssize_t send_wait(int fd, const void *buf, size_t count)
{
	size_t rem = count;
	while (rem > 0) {
		ssize_t ret = send(fd, buf, rem, 0);
		if (ret < 0)
			return -errno;
		if (ret == rem)
			break;
		buf += ret;
		rem -= ret;
		usleep(10000);
	}
	return count;
}

//-----------------------------------------------------------------------------
static inline long K_VERSION(void)
{
	pthread_mutex_lock(&k_version_lock);
	if (k_version_data == 0)
		setup_kversion();
	pthread_mutex_unlock(&k_version_lock);
	return k_version_data;
}

static void setup_kversion(void)
{
	int x, y, z;
	FILE *fp;
	if ((fp = fopen("/proc/version", "r")) == NULL)
		return;
	if (fscanf(fp, "%*s %*s %d.%d.%d", &x, &y, &z) != 3)
		return;
	fclose(fp);
	k_version_data = KERNEL_VERSION(x, y, z);
	return;
}

//=============================================================================

/*
 *	ttyrpld/user/infod.c
 *	Copyright © Jan Engelhardt <jengelh [at] medozas de>, 2004 - 2008
 *
 *	This file is part of ttyrpld. ttyrpld is free software; you can
 *	redistribute it and/or modify it under the terms of the GNU
 *	Lesser General Public License as published by the Free Software
 *	Foundation; either version 2 or 3 of the License.
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include <libHX/arbtree.h>
#include <libHX/string.h>
#include "rpl_stdint.h"
#include "dev.h"
#include "lib.h"
#include "rdsh.h"

/* Functions */
static void *client_thread(void *);
static void set_session_status(uint32_t, int);
static void getinfo_text(uint32_t, int);
static void getinfo_text_all(int);
static void getinfo_text_one(int, struct tty *);
static void getinfo_bin(uint32_t, int);
static void getinfo_bin_all(int);
static void getinfo_bin_one(int, struct tty *);
static void zero_counters(uint32_t);

static inline const char *basename_pp(const char *, const char *);
static void block_signals(void);
static int skprintf(int, const char *, ...);
static int unix_server(const char *);

/* Variables */
static int Svfd = -1;

//-----------------------------------------------------------------------------
void infod_init(void)
{
	/*
	 * bind() in unix_server() needs to be called as root (since in the
	 * default case we will create the socket in /var/run/), so
	 * init_infod() must come before setuid().
	 */
	Svfd = unix_server(GOpt.infod_port);

	/*
	 * Limit the connections to the user owning the socket (and root). The
	 * Kernel code for AF_UNIX sockets only requires write permission on
	 * the socket for that.
	 */
	chown(GOpt.infod_port, GOpt.user_id, 0);
	chmod(GOpt.infod_port, S_IWUSR);
	return;
}

void *infod_main(void *arg)
{
	/* The INFOD mainloop which waits for incoming connections. */
	block_signals();

	while (true) {
		socklen_t sz = sizeof(struct sockaddr_un);
		struct sockaddr_un remote;
		pthread_t id;
		int clfd;

		if ((clfd = accept(Svfd, reinterpret_cast(void *,
		    &remote), &sz)) < 0)
			continue;

		pthread_create(&id, NULL, client_thread,
		               reinterpret_cast(void *, static_cast(long, clfd)));
		pthread_detach(id);
	}

	return NULL;
}

static void *client_thread(void *arg)
{
	int fd = reinterpret_cast(long, arg);

	while (true) {
		static const uint32_t zero = 0;
		unsigned char req;
		uint32_t gint;
		bool ret = false;

		ret |= !RECEIVE_FULL(fd, &req, sizeof(unsigned char));
		ret |= !RECEIVE_FULL(fd, &gint, sizeof(gint));
		SWAB1(&gint);
		if (ret || req == IFP_NONE)
			break;

		switch (req) {
		case IFP_ACTIVATE ... IFP_DEACTIVSES:
			set_session_status(gint, req);
			break;
		case IFP_REMOVE: {
			struct tty *tty;
			pthread_mutex_lock(&Ttys_lock);
			if ((tty = get_tty(gint, false)) != NULL)
				log_close(tty);
			pthread_mutex_unlock(&Ttys_lock);
			break;
		}
		case IFP_GETINFO:
			getinfo_bin(gint, fd);
			break;
		case IFP_GETINFO_T:
			getinfo_text(gint, fd);
			break;
		case IFP_ZERO:
			zero_counters(gint);
			break;
		default:
			notify(LOG_WARNING, _("[infod] Unknown "
			       "request %d, closing connection to not "
			       "run into conversion problems.\n"),
			       req);
			goto clthr__exit;
		} /* switch */

		send_wait(fd, &zero, sizeof(uint32_t));
	}

 clthr__exit:
	close(fd);
	return NULL;
}

//-----------------------------------------------------------------------------
#ifdef __linux__
#	define ARCH "Linux"
#elif defined(__FreeBSD__)
#	define ARCH "FreeBSD"
#elif defined(__OpenBSD__)
#	define ARCH "OpenBSD"
#elif defined(__NetBSD__)
#	define ARCH "NetBSD"
#elif defined(__sun__)
#	define ARCH "SunOS"
#else
#	define ARCH "Other"
#endif

static void getinfo_text(uint32_t dev, int fd)
{
	pthread_mutex_lock(&Ttys_lock);
	if (dev == 0) {
		getinfo_text_all(fd);
	} else {
		struct tty *tty;
		if ((tty = get_tty(dev, false)) == NULL) {
			pthread_mutex_unlock(&Ttys_lock);
			return;
		}
		getinfo_text_one(fd, tty);
	}

	pthread_mutex_unlock(&Ttys_lock);
	return;
}

static void getinfo_text_all(int fd)
{
	/* No device given, send info about RPLD and all ttys */
	const struct HXbtree_node *node;
	struct HXbtrav *trav;

	skprintf(fd,
	"=========================================================="
	"================\n"
	"  (bytes)     %9llu %9llu ttyrpld " TTYRPLD_VERSION "/" ARCH "\n"
	"  (packets)   %9lu %9lu\n"
	"----------------------------------------------------------"
	"----------------\n",
	Stats.in, Stats.out, Stats.read, Stats.write);

	trav = HXbtrav_init(Ttys);
	while ((node = HXbtraverse(trav)) != NULL)
		getinfo_text_one(fd, node->data);

	skprintf(fd, "-----------------------------------------------"
	"---------------------------\n");
	HXbtrav_free(trav);
	return;
}

static void getinfo_text_one(int fd, struct tty *tty)
{
	char status = '?';
	switch (tty->status) {
		case IFP_ACTIVATE:   status = 'A'; break;
		case IFP_DEACTIVATE: status = 'D'; break;
		case IFP_DEACTIVSES: status = 'S'; break;
	}
	skprintf(fd, "%c %-11s %9lu %9lu %s\n", status, tty->sdev, tty->in,
	         tty->out, basename_pp(tty->log, GOpt.ofmt));
	return;
}

static void getinfo_bin(uint32_t dev, int fd)
{
	pthread_mutex_lock(&Ttys_lock);
	if (dev == 0) {
		getinfo_bin_all(fd);
	} else {
		struct tty *tty;
		if ((tty = get_tty(dev, false)) == NULL) {
			pthread_mutex_unlock(&Ttys_lock);
			return;
		}
		getinfo_bin_one(fd, tty);
	}

	pthread_mutex_unlock(&Ttys_lock);
	return;
}

static void getinfo_bin_all(int fd)
{
	/* No device given, send info about RPLD and all ttys */
	const struct HXbtree_node *node;
	struct HXbtrav *trav;

	skprintf(fd, "ttyrpld " TTYRPLD_VERSION "\n" "format 4\n"
	         "%lu %lu %lu %lu %llu %llu %lu\n",
	         Stats.open, Stats.lclose, Stats.read, Stats.write,
	         Stats.in, Stats.out, Stats.badpack);

	trav = HXbtrav_init(Ttys);
	while ((node = HXbtraverse(trav)) != NULL)
		getinfo_bin_one(fd, node->data);

	HXbtrav_free(trav);
	return;
}

static void getinfo_bin_one(int fd, struct tty *tty)
{
	char status = '?';
	switch (tty->status) {
		case IFP_ACTIVATE:   status = 'A'; break;
		case IFP_DEACTIVATE: status = 'D'; break;
		case IFP_DEACTIVSES: status = 'S'; break;
	}
	skprintf(fd, "%c %ld %ld %s %lu %lu %s\n", status,
	         KD26_PARTS(tty->dev), tty->sdev, tty->in, tty->out, tty->log);
	return;
}

static void set_session_status(uint32_t dev, int req)
{
	struct tty *tty;

	pthread_mutex_lock(&Ttys_lock);
	if ((tty = get_tty(dev, true)) == NULL) {
		pthread_mutex_unlock(&Ttys_lock);
		return;
	}

	tty->status = req;
	pthread_mutex_unlock(&Ttys_lock);
	return;
}

static void zero_counters(uint32_t dev)
{
	pthread_mutex_lock(&Ttys_lock);
	if (dev == 0) {
		struct HXbtrav *trav = HXbtrav_init(Ttys);
		const struct HXbtree_node *node;

		while ((node = HXbtraverse(trav)) != NULL) {
			struct tty *tty = node->data;
			tty->in = tty->out = 0;
		}

		memset(&Stats, 0, sizeof(Stats));
		HXbtrav_free(trav);
	} else {
		struct tty *tty;
		if ((tty = get_tty(dev, false)) == NULL) {
			pthread_mutex_unlock(&Ttys_lock);
			return;
		}
		tty->in = tty->out = 0;
	}
	pthread_mutex_unlock(&Ttys_lock);
	return;
}

//-----------------------------------------------------------------------------
static inline const char *basename_pp(const char *s, const char *t)
{
	const char *p;
	if (s == NULL)
		return NULL;

	if ((p = strchr(t, '%')) != NULL) {
		if ((p = HX_strbchr(t, p, '/')) != NULL)
			return s + (p - t) + 1;
		return s;
	}

	return HX_basename(s);
}

static void block_signals(void)
{
	sigset_t set;
	sigfillset(&set);
	pthread_sigmask(SIG_BLOCK, &set, NULL);
	return;
}

static int skprintf(int fd, const char *fmt, ...)
{
	va_list argp;
	char buf[512];
	uint32_t le, s;

	va_start(argp, fmt);
	le = s = vsnprintf(buf, sizeof(buf), fmt, argp);
	SWAB1(&le);
	send_wait(fd, &le, sizeof(uint32_t));
	send_wait(fd, buf, s);

	va_end(argp);
	return s;
}

static int unix_server(const char *port)
{
	struct sockaddr_un sk;
	int fd = -1;

	sk.sun_family = AF_UNIX;
	strncpy(sk.sun_path, port, sizeof(sk.sun_path));
	sk.sun_path[sizeof(sk.sun_path) - 1] = '\0';
	unlink(sk.sun_path);

	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 ||
	    bind(fd, reinterpret_cast(struct sockaddr *, &sk),
	    sizeof(struct sockaddr_un)) < 0 ||
	    listen(fd, SOMAXCONN) < 0) {
		close(fd);
		return -1;
	}

	return fd;
}

//=============================================================================

/*
 *	ttyrpld/user/rpld.c
 *	Copyright © Jan Engelhardt <jengelh [at] medozas de>, 2004 - 2008
 *
 *	This file is part of ttyrpld. ttyrpld is free software; you can
 *	redistribute it and/or modify it under the terms of the GNU
 *	Lesser General Public License as published by the Free Software
 *	Foundation; either version 2 or 3 of the License.
 */
#include "config.h"
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef HAVE_ALLOCA
#	include <alloca.h>
#endif
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <pwd.h>
#include <signal.h>
#include <stdbool.h>
#include "rpl_stdint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#include <libHX/arbtree.h>
#include <libHX/misc.h>
#include <libHX/option.h>
#include <libHX/string.h>
#include "compat.h"
#include "dev.h"
#include "rpl_endian.h"
#include "rpl_ioctl.h"
#include "rpl_packet.h"
#include "lib.h"
#include "rdsh.h"

/* Definitions */
enum {
	C_LOGOPEN = 0,
	C_BADPACKET,
	C_PKTTYPE,
	C_KERNEL,
	C_MAX,
};

/* Functions */
static void mainloop(int);

static bool packet_preprox(struct rpldev_packet *);
static bool packet_process(struct rpldev_packet *, struct tty *, int);

static void evt_open(struct rpldev_packet *, struct tty *, int);
static void log_open(struct tty *);
static void log_write(struct rpldev_packet *, struct tty *, int);

static int check_parent_directory(const hxmc_t *);
static void fill_info(struct tty *, const char *);

static int init_device(const char *);
static void init_fdtable(rlim_t);
static int init_sighandler(void);
static void sighandler_int(int);
static void sighandler_alrm(int);
static void sighandler_pipe(int);

static bool find_devnode(uint32_t, char *, size_t, const char **);
static bool find_devnode_dive(uint32_t, char *, size_t, const char *);
static char *getnamefromuid(uid_t, char *, size_t);
static uid_t getuidfromname(const char *);
static bool get_options(int *, const char ***);
static void getopt_config(const struct HXoptcb *);
static void getopt_username(const struct HXoptcb *);
static bool rate_limit(int, time_t);
static bool read_config(const char *);
static bool read_config_bp(const char *, const char *);

/* Variables */
static struct {
	char *device;
	long bsize;
	int max_fd;
	unsigned int dolog, infod_start;
	bool _running;
} Opt = {
	._running    = true,
	.bsize       = 32 * 1024,
	.device      = "/dev/misc/rpl /dev/rpl /devices/pseudo/rpldev@0:0",
	.dolog       = true,
	.infod_start = false,
	.max_fd      = -1, /* use system default */
};

//-----------------------------------------------------------------------------
int main(int argc, const char **argv)
{
	pthread_t infod_id;
	int fd;

	load_locale(*argv);
	umask(~(S_IRUSR | S_IWUSR | S_IXUSR));

	/*
	 * Yep, the config file is what is needed by all three
	 * (rpld, infod, rplctl).
	 */
	if (!read_config("/etc/rpld.conf") && errno != ENOENT)
		fprintf(stderr, _("/etc/rpld.conf exists but could not be "
                        "read: %s\n"), strerror(errno));
	if (!read_config("/usr/local/etc/rpld.conf") && errno != ENOENT)
		fprintf(stderr, _("/usr/local/etc/rpld.conf exists but could "
		        "not be read: %s\n"), strerror(errno));
	if (!read_config_bp(*argv, "rpld.conf") && errno != ENOENT)
		fprintf(stderr, _("$BINPATH/rpld.conf exists but could not be"
		        " read: %s\n"), strerror(errno));

	if (strcmp(HX_basename(*argv), "rplctl") == 0)
		return rplctl_main(argc, argv);

	if (!get_options(&argc, &argv))
		return EXIT_FAILURE;
	memset(&Stats, 0, sizeof(Stats));

	if (GOpt.verbose) {
		printf("# rpld " TTYRPLD_VERSION "\n");
		printf(_(
		       "This program comes with ABSOLUTELY NO WARRANTY; it is free software and you\n"
		       "you are welcome to redistribute it under certain conditions; for details see\n"
		       "the \"LICENSE.GPL2\" file which should have come with this program.\n"
		));
		printf("\n");
	}

	if ((Ttys = HXbtree_init(HXBT_MAP | HXBT_ICMP)) == NULL) {
		perror("Ttys = HXbtree_init()");
		return EXIT_FAILURE;
	}

	if (Opt.max_fd > 4)
		init_fdtable(Opt.max_fd);

	if ((fd = init_device(Opt.device)) < 0) {
		fprintf(stderr, _("No device could be opened, aborting.\n"));
		return EXIT_FAILURE;
	}

	init_sighandler();
	if (Opt.infod_start)  infod_init();
	if (GOpt.syslog)      openlog("rpld", LOG_PID, LOG_DAEMON);
	if (GOpt.user_id > 0) setuid(GOpt.user_id);
	if (Opt.infod_start)  pthread_create(&infod_id, NULL, infod_main, NULL);
	if (GOpt.verbose)     alarm(1);
	mainloop(fd);

	if (Opt.infod_start) {
		unlink(GOpt.infod_port);
		pthread_cancel(infod_id);
		pthread_join(infod_id, NULL);
	}

	close(fd);
	return EXIT_SUCCESS;
}

static void mainloop(int fd)
{
    while (Opt._running) {
	struct rpldev_packet packet;
	struct tty *tty;
	ssize_t ret;

	if ((ret = read(fd, &packet, sizeof(struct rpldev_packet))) <
	    static_cast(ssize_t, sizeof(struct rpldev_packet))) {
		struct stat sb;
#if defined(__OpenBSD__) || defined(__NetBSD__)
		if (errno == EINTR)
			continue;
#endif
		fstat(fd, &sb);
		if (!S_ISREG(sb.st_mode))
			fprintf(stderr, _("\n" "Short read: %ld bytes only. "
					"Error %d: %s\n"), static_cast(long, ret), errno,
					strerror(errno));
		Opt._running = 0;
		break;
	}

	SWAB1(&packet.dev);
	SWAB1(&packet.size);
	SWAB1(&packet.time.tv_sec);
	SWAB1(&packet.time.tv_usec);

	if (packet.magic != MAGIC_SIG) {
		++Stats.badpack;
		if (rate_limit(C_BADPACKET, 2))
			notify(LOG_WARNING,
			       _("Bogus packet (magic is 0x%02X)!\n"),
			       packet.magic);
		continue;
	}

	if (!packet_preprox(&packet)) {
		G_skip(fd, packet.size, 0);
		continue;
	}

	pthread_mutex_lock(&Ttys_lock);
	if ((tty = get_tty(packet.dev, true)) == NULL) {
		G_skip(fd, packet.size, 0);
		pthread_mutex_unlock(&Ttys_lock);
		continue;
	}

	if (!packet_process(&packet, tty, fd))
		/*
		 * packet_process() always succeeds, but it returns 0 to
		 * indicate if it wants to skip the payload.
		 */
		G_skip(fd, packet.size, 0);

	pthread_mutex_unlock(&Ttys_lock);
    }

    return;
}

//-----------------------------------------------------------------------------
static bool packet_preprox(struct rpldev_packet *packet)
{
	static unsigned long *const tab[] = {
		[EVT_OPEN]   = &Stats.open,
		[EVT_READ]   = &Stats.read,
		[EVT_WRITE]  = &Stats.write,
		[EVT_LCLOSE] = &Stats.lclose,
		[EVT_max]    = NULL,
	};

	if (packet->event < EVT_max && tab[packet->event] != NULL)
		++*tab[packet->event];

	/* General packet classification (first stage drop) */
	switch (packet->event) {
	/* These will be processed */
	case EVT_OPEN:
	case EVT_LCLOSE:
		break;

	/* The following roll their own and will also be processed... */
	case EVT_READ:
		Stats.in += packet->size;
		break;
	case EVT_WRITE:
		Stats.out += packet->size;
		break;
	default:
		if (rate_limit(C_PKTTYPE, 2))
			notify(LOG_WARNING,
			       _("Unknown packet type 0x%02X\n"),
			       packet->event);
		return false;
	} /* switch */

	return true;
}

static bool packet_process(struct rpldev_packet *packet,
    struct tty *tty, int fd)
{
	if (tty->status == IFP_DEFAULT) {
		fill_info(tty, NULL);
		tty->status = Opt.dolog ? IFP_ACTIVATE : IFP_DEACTIVATE;
	}

	if (tty->status != IFP_ACTIVATE) {
		switch (packet->event) {
			case EVT_READ:
				tty->in += packet->size;
				break;
			case EVT_WRITE:
				tty->out += packet->size;
				break;
		}
		return false;
	}

	switch (packet->event) {
		case EVT_OPEN:
			evt_open(packet, tty, fd);
			return true;
		case EVT_READ:
			tty->in += packet->size;
			log_write(packet, tty, fd);
			return true;
		case EVT_WRITE:
			tty->out += packet->size;
			log_write(packet, tty, fd);
			return true;
		case EVT_LCLOSE:
			log_close(tty);
			break;
		default:
			notify(LOG_ERR, _("Should never get here! (%s:%d) "
			       "Forgot to code something? (event=%d)\n"),
			       __FILE__, __LINE__, packet->event);
			break;
	}

	return false;
}

//-----------------------------------------------------------------------------
static void evt_open(struct rpldev_packet *packet, struct tty *tty, int fd)
{
	/*
	 * OPEN event:
	 * - Read the dentry from rpldev to save looking up the device node
	 * - Fill basic variables (.sdev and .log)
	 * - Do NOT open the logfile
	 * - Find out about UID changes on the device and start a new logfile
	 *   if owner has changed.
	 */
	char *sdev = alloca(packet->size + 1);
	bool owner_changed = false, fill_it = false;
	struct stat sb;

	read(fd, sdev, packet->size);
	sdev[packet->size] = '\0';

	if (tty->sdev == NULL)
		fill_it = true;

	if (tty->uid != -1 && tty->full_dev != NULL &&
	    stat(tty->full_dev, &sb) == 0 && sb.st_uid != tty->uid) {
		/* Create new logfile if owner changed */
		tty->in	      = tty->out = 0;
		owner_changed = true;
		fill_it       = true;
	}

	if (fill_it)
		fill_info(tty, sdev);
	if (owner_changed)
		log_open(tty);

	return;
}

static void log_open(struct tty *tty)
{
	struct rpldsk_packet p = {.magic = MAGIC_SIG, .time = {-1, -1}};
	struct tm now_tm, *nowp;
	char buf[MAXFNLEN];
	time_t now_sec;
	size_t s;

	if (check_parent_directory(tty->log) <= 0 && rate_limit(C_LOGOPEN, 5))
		notify(LOG_ERR, _("Directory permission denied: It won't be "
		       "possible to write to the file %s, expect warnings.\n"),
		       tty->log);

	if (tty->fd >= 0)
		close(tty->fd);

	tty->fd = open(tty->log, O_WRONLY | O_CREAT | O_APPEND,
	          S_IRUSR | S_IWUSR);

	/*
	 * Add an optional magic packet for file(1) to recognize. (EVT_ID_PROG
	 * may contain _anything_, while EVT_MAGIC is fixed. See
	 * share/ttyrpld.magic.)
	 */
	HX_strlcpy(buf, "RPL2_50", sizeof(buf)); /* STRING FIXED */
	p.event = EVT_MAGIC;
	s = p.size = strlen(buf) + 1; /* include '\0' in stream */
	SWAB1(&p.size);
	write(tty->fd, &p, sizeof(struct rpldsk_packet));
	write(tty->fd, buf, s);

	/*
	 * Add an optional ident header to record the program and version which
	 * this logfile was created with.
	 */
	HX_strlcpy(buf, "ttyrpld " TTYRPLD_VERSION, sizeof(buf));
	p.event = EVT_ID_PROG;
	s = p.size = strlen(buf) + 1;
	SWAB1(&p.size);
	write(tty->fd, &p, sizeof(struct rpldsk_packet));
	write(tty->fd, buf, s);

	/* Also add the timestamp this log was created */
	now_sec = time(NULL);
	nowp = localtime_r(&now_sec, &now_tm);
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", nowp);
	p.event = EVT_ID_TIME;
	s = p.size = strlen(buf) + 1;
	SWAB1(&p.size);
	write(tty->fd, &p, sizeof(struct rpldsk_packet));
	write(tty->fd, buf, s);

	/* ... and the full path name of the tty for reference. */
	p.event = EVT_ID_DEVPATH;
	s = p.size = strlen(tty->full_dev) + 1;
	SWAB1(&p.size);
	write(tty->fd, &p, sizeof(struct rpldsk_packet));
	write(tty->fd, tty->full_dev, s);

	/* ... as well as the username (or UID) the tty belongs to */
	if (getnamefromuid(tty->uid, buf, sizeof(buf)) == NULL)
		snprintf(buf, sizeof(buf), "%ld", static_cast(long, tty->uid));
	p.event = EVT_ID_USER;
	s = p.size = strlen(buf) + 1;
	SWAB1(&p.size);
	write(tty->fd, &p, sizeof(struct rpldsk_packet));
	write(tty->fd, buf, s);

	return;
}

static void log_write(struct rpldev_packet *packet, struct tty *tty, int fd)
{
	char *buffer = alloca(packet->size);
	ssize_t have;

	if (tty->fd < 0)
		log_open(tty);
	if ((have = read(fd, buffer, packet->size)) <= 0)
		return;
	if (have != packet->size)
		packet->size = have;

	SWAB1(&packet->size);
	SWAB1(&packet->time.tv_sec);
	SWAB1(&packet->time.tv_usec);
	write(tty->fd, &packet->size, sizeof(struct rpldsk_packet));
	write(tty->fd, buffer, have);
	return;
}

//-----------------------------------------------------------------------------
static int check_parent_directory(const hxmc_t *s)
{
	char *path = alloca(strlen(s) + 1), *p;

	strcpy(path, s);
	if ((p = strrchr(path, '/')) == NULL)
		/* Current dirctory, no more dir checks needed */
		return 1;

	*p = '\0';
	return HX_mkdir(path); /* `mkdir -p` */
}

static void fill_info(struct tty *tty, const char *aux_sdev)
{
	struct HXbtree *catalog;
	char full_dev[MAXFNLEN], sdev[MAXFNLEN],
		fmday[16], fmtime[16], user[64];
	const char *pbase = NULL;
	struct stat sb;
	int i = 0;

	/*
	 * The rpldev kernel module provides us with the real dentry name
	 * (aux_sdev) that was used open the device. Use it, if available.
	 */
	if (aux_sdev != NULL && *aux_sdev != '\0') {
		const char **dirp = Device_dirs;
		HX_strlcpy(full_dev, aux_sdev, sizeof(full_dev));
		while (*dirp != NULL) {
			if (strncmp(full_dev, *dirp, strlen(*dirp)) == 0) {
				pbase = *dirp;
				break;
			}
			++dirp;
		}
		if (pbase == NULL && *full_dev == '/')
			pbase = "";
	} else if (!find_devnode(tty->dev, full_dev,
	    sizeof(full_dev), &pbase)) {
		/*
		 * Use [MAJOR:MINOR] as a fictitious filename if the device
		 * node could not be found.
		 */
		snprintf(full_dev, sizeof(full_dev), "[%lu:%lu]",
		         KD26_PARTS(tty->dev));
	}

	/*
	 * rpld is able to sort logs by user (by putting each user's logfiles
	 * into a separate directory) -- for that, we need the username.
	 */
	if (stat(full_dev, &sb) < 0) {
		/* This will happen if we get a [MAJOR:MINOR] name... */
		strcpy(user, _("NONE"));
	} else {
		tty->uid = sb.st_uid;
		if (getnamefromuid(sb.st_uid, user, sizeof(user)) == NULL)
			/* Well, at least the UID. */
			snprintf(user, sizeof(user), "%ld",
			         static_cast(long, sb.st_uid));
	}

	/*
	 * The filename in sdev contains a common prefix, such as "/dev", and
	 * may contain further slashes, e.g. as in "pts/2". We must exchange
	 * them since a filename cannot contain a slash -- it would otherwise
	 * always be treated as another directory component.
	 */
	if (pbase != NULL)
		/* only copy "pts/2" part - copy includes '\0' */
		memmove(sdev, full_dev + strlen(pbase) + 1,
		        strlen(full_dev) - strlen(pbase));
	else
		/* Usually this is [MAJOR:MINOR] */
		HX_strlcpy(sdev, full_dev, sizeof(sdev));

	while (i < sizeof(sdev) && sdev[i] != '\0') {
		if (sdev[i] == '/')
			sdev[i] = '-';
		++i;
	}

	/*
	 * Keep the device path that was used first. I do this because the
	 * major-minor may stay the same, but the device node may be named
	 * differently. (I know this does not happen, but let's have it.)
	 */
	if (tty->sdev == NULL) {
		HX_strclone(&tty->sdev, sdev);
		HX_strclone(&tty->full_dev, full_dev);
	}

	{ /* What would be a logfile without a proper timestamp? */
		time_t now = time(NULL);
		struct tm now_tm;
		localtime_r(&now, &now_tm);
		strftime(fmday,  sizeof(fmday),  "%Y%m%d", &now_tm);
		strftime(fmtime, sizeof(fmtime), "%H%M%S", &now_tm);
	}

	HXmc_free(tty->log);
	catalog  = HXformat_init();
	HXformat_add(catalog, "DATE", fmday,  HXTYPE_STRING);
	HXformat_add(catalog, "TIME", fmtime, HXTYPE_STRING);
	HXformat_add(catalog, "TTY",  sdev,   HXTYPE_STRING);
	HXformat_add(catalog, "USER", user,   HXTYPE_STRING);
	HXformat_aprintf(catalog, &tty->log, GOpt.ofmt);
	HXformat_free(catalog);
	return;
}

//-----------------------------------------------------------------------------
static int init_device(const char *in_devs)
{
	char *copy = HX_strdup(in_devs), *workp = copy, *devp;
	int fd = -1;

	while ((devp = HX_strsep(&workp, " ")) != NULL) {
		if (devp[0] == '-' && devp[1] == '\0') {
			fd = STDIN_FILENO;
			if (GOpt.verbose)
				printf(_("Connected to %s\n"), "<stdin>");
			break;
		}
		if ((fd = open(devp, O_RDONLY)) >= 0) {
			if (GOpt.verbose)
				printf(_("Connected to %s\n"), devp);
			break;
		}
		if (errno != ENOENT) {
			if (errno == EACCES)
				fprintf(stderr, _("The device should be owned "
				        "by the user running rpld (UID %ld) "
				        "and have mode 0400.\n"),
				        GOpt.user_id);
			fprintf(stderr, _("static_find(): Could not open %s "
			        "even though it exists: %s (trying next "
			        "device)\n"), devp, strerror(errno));
		} else if (errno == EBUSY) {
			fprintf(stderr, _("\t" "The RPL device can only be "
			        "opened once,\n\t" "there is probably an "
			        "instance of rpld running!\n"));
		}
	}

	free(copy);
	return fd;
}

static void init_fdtable(rlim_t nfd)
{
	struct rlimit rl = {
		.rlim_cur = nfd,
		.rlim_max = nfd,
	};
	if (setrlimit(RLIMIT_NOFILE, &rl) != 0)
		fprintf(stderr, _("Warning: Could not increase fd table size "
		        "to %d: %s\n"), (int)nfd, strerror(errno));
	return;
}

static int init_sighandler(void)
{
	struct sigaction s_int, s_alrm, s_pipe;

	s_int.sa_handler = sighandler_int;
	s_int.sa_flags   = SA_RESTART;
	sigemptyset(&s_int.sa_mask);

	s_alrm.sa_handler = sighandler_alrm;
	s_alrm.sa_flags   = SA_RESTART;
	sigemptyset(&s_alrm.sa_mask);

	s_pipe.sa_handler = sighandler_pipe;
	s_pipe.sa_flags   = SA_RESTART;
	sigemptyset(&s_pipe.sa_mask);

	/*
	 * All sigactions() shall be executed, however, if one fails, this
	 * function shall return <= 0, otherwise >0 upon success.  Geesh, I
	 * love these constructs.
	 */
	return !(!!sigaction(SIGINT, &s_int, NULL) +
	 !!sigaction(SIGTERM, &s_int, NULL) +
	 !!sigaction(SIGALRM, &s_alrm, NULL) +
	 !!sigaction(SIGPIPE, &s_pipe, NULL));
}

static void sighandler_int(int s)
{
	if (Opt._running-- == 0) {
		if (GOpt.verbose)
			printf(_("Second time we received SIGINT/SIGTERM,"
			 " exiting immediately.\n"));
		exit(EXIT_FAILURE);
	}
	if (GOpt.verbose)
		printf(_("\n" "Received SIGINT/SIGTERM, shutting down.\n"));
	Opt._running = 0;
	return;
}

static void sighandler_alrm(int s)
{
	printf("\r\e[2K" "read %lluB/%luP, write %lluB/%luP",
	       Stats.in, Stats.read, Stats.out, Stats.write);
	fflush(stdout);
	if (GOpt.verbose)
		alarm(1);
	return;
}

static void sighandler_pipe(int s)
{
	fprintf(stderr, _("\n" "[%d] Received SIGPIPE\n"),
	        static_cast(int, getpid()));
	return;
}

//-----------------------------------------------------------------------------
static bool find_devnode(uint32_t id, char *dest, size_t len,
    const char **loc_pbase)
{
	/*
	 * Walk through the list of directories containing all the device nodes
	 * and compare their major/minor numbers with ID. Note that we will not
	 * search in arbitrary locations, such as chroot jails.
	 */
	const char **dirp = Device_dirs;
	while (*dirp != NULL) {
		if (find_devnode_dive(id, dest, len, *dirp)) {
			if (loc_pbase != NULL)
				*loc_pbase = *dirp;
			return true;
		}
		++dirp;
	}
	return false;
}

static bool find_devnode_dive(uint32_t id, char *dest, size_t len,
    const char *dir)
{
	/*
	 * Scan a directory for node. During directory traversal, everything
	 * that begins in a dot is ignored -- this is crude behavior, but the
	 * simplicity is justified given that there are no device nodes
	 * starting with a dot. And if, you are either special or there is a
	 * good reason.
	 */
	char buf[MAXFNLEN];
	struct stat sb_self, sb_deref;
	const char *de;
	bool ret = false;
	void *dx;

	if ((dx = HXdir_open(dir)) == NULL) {
		if (errno != ENOENT && rate_limit(C_KERNEL, 10))
			notify(LOG_WARNING, _("Could not open %s: %s\n"),
			       dir, strerror(errno));
		return false;
	}

	while ((de = HXdir_read(dx)) != NULL) {
		if (*de == '.' || strcmp(de, "stdout") == 0 ||
		    strcmp(de, "stderr") == 0 || strcmp(de, "stdin") == 0)
			continue;
		snprintf(buf, sizeof(buf), "%s/%s", dir, de);

		if (lstat(buf, &sb_self) < 0 || stat(buf, &sb_deref) < 0)
			continue;
		if (S_ISCHR(sb_deref.st_mode) &&
		    K26_MKDEV(COMPAT_MAJOR(sb_deref.st_rdev),
		    COMPAT_MINOR(sb_deref.st_rdev)) == id) {
			HX_strlcpy(dest, buf, len);
			ret = true;
			break;
		} else if (!S_ISLNK(sb_self.st_mode) &&
		    S_ISDIR(sb_deref.st_mode)) {
			snprintf(buf, sizeof(buf), "%s/%s", dir, de);
			if (!find_devnode_dive(id, dest, len, buf))
				continue;
			ret = true;
			break;
		}
	}

	HXdir_close(dx);
	return ret;
}

static char *getnamefromuid(uid_t uid, char *result, size_t len)
{
	/* Turn a UID into a username, if possible. */
	struct passwd ent, *ep;
	char additional[1024];
	ep = rpld_getpwuid(uid, &ent, additional, sizeof(additional));
	if (ep == NULL)
		return NULL;
	HX_strlcpy(result, ep->pw_name, len);
	return result;
}

static uid_t getuidfromname(const char *name)
{
	struct passwd ent, *ep;
	char additional[1024];
	ep = rpld_getpwnam(name, &ent, additional, sizeof(additional));
	if (ep == NULL)
		return -1;
	return ep->pw_uid;
}

static bool get_options(int *argc, const char ***argv)
{
	struct HXoption options_table[] = {
	        {.sh = 'D', .type = HXTYPE_STRING, .ptr = &Opt.device,
	         .help = _("Path to the RPL device"), .htyp = _("file")},
	        {.sh = 'I', .type = HXTYPE_VAL, .ptr = &Opt.infod_start,
		 .val = true, .help = _("Make statistics available through socket")},
	        {.sh = 'i', .type = HXTYPE_VAL, .ptr = &Opt.infod_start,
		 .val = false, .help = _("Do not make statistics available")},
	        {.sh = 'O', .type = HXTYPE_STRING, .ptr = &GOpt.ofmt,
	         .help = _("Override OFMT variable"), .htyp = _("string")},
	        {.sh = 'Q', .type = HXTYPE_VAL, .ptr = &Opt.dolog, .val = false,
	         .help = _("Deactivate logging, only do bytecounting")},
	        {.sh = 'U', .type = HXTYPE_STRING, .cb = getopt_username,
	         .help = _("User to change to"), .htyp = _("user")},
	        {.sh = 'c', .type = HXTYPE_STRING, .cb = getopt_config,
	         .help = _("Read configuration from file"), .htyp = _("file")},
	        {.sh = 'i', .type = HXTYPE_VAL, .ptr = &Opt.infod_start, .val = false,
	         .help = _("Do not start INFOD subcomponent")},
	        {.sh = 's', .type = HXTYPE_NONE, .ptr = &GOpt.syslog,
	         .help = _("Print warnings/errors to syslog")},
	        {.sh = 'v', .type = HXTYPE_NONE | HXOPT_INC, .ptr = &GOpt.verbose,
	         .help = _("Print statistics while rpld is running (overrides -s)")},
	        HXOPT_AUTOHELP,
	        HXOPT_TABLEEND,
	};

	return HX_getopt(options_table, argc, argv, HXOPT_USAGEONERR) > 0;
}

static void getopt_config(const struct HXoptcb *cbi)
{
	read_config(cbi->data);
	return;
}

static void getopt_username(const struct HXoptcb *cbi)
{
	if ((GOpt.user_id = getuidfromname(cbi->data)) < 0) {
		fprintf(stderr, _("No such user: %s\n"), cbi->data);
		exit(EXIT_FAILURE);
	}
	return;
}
	
static bool rate_limit(int counter, time_t delta)
{
	static time_t last_time[C_MAX] = {};
	time_t now = time(NULL);

	if (now > last_time[counter] + delta) {
		last_time[counter] = now;
		return true;
	}

	return false;
}

static bool read_config(const char *file)
{
	static const struct HXoption config_table[] = {
	        {.ln = "DEVICE",      .type = HXTYPE_STRING, .ptr = &Opt.device},
	        {.ln = "DO_LOG",      .type = HXTYPE_BOOL,   .ptr = &Opt.dolog},
	        {.ln = "INFOD_PORT",  .type = HXTYPE_STRING, .ptr = &GOpt.infod_port},
	        {.ln = "MAX_FD",      .type = HXTYPE_INT,    .ptr = &Opt.max_fd},
	        {.ln = "START_INFOD", .type = HXTYPE_BOOL,   .ptr = &Opt.infod_start},
	        {.ln = "OFMT",        .type = HXTYPE_STRING, .ptr = &GOpt.ofmt},
	        {.ln = "USER",        .type = HXTYPE_NONE,   .ptr = &GOpt.user_id,
	         .cb = getopt_username},
		HXOPT_TABLEEND,
	};
	return HX_shconfig(file, config_table) > 0;
}

static bool read_config_bp(const char *app_path, const char *file)
{
	char *fpath = HX_strdup(app_path), *ptr, construct[MAXFNLEN];
	if ((ptr = strrchr(fpath, '/')) == NULL) {
		HX_strlcpy(construct, file, sizeof(construct));
	} else {
		*ptr++ = '\0';
		snprintf(construct, sizeof(construct), "%s/%s", fpath, file);
	}
	free(fpath);
	return read_config(construct);
}

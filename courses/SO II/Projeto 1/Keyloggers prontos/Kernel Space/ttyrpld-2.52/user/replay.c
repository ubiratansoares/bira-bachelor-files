/*
 *	ttyrpld/user/replay.c
 *	Copyright © Jan Engelhardt <jengelh [at] medozas de>, 2004 - 2008
 *
 *	This file is part of ttyrpld. ttyrpld is free software; you can
 *	redistribute it and/or modify it under the terms of the GNU
 *	Lesser General Public License as published by the Free Software
 *	Foundation; either version 2 or 3 of the License.
 */
#include <sys/stat.h>
#include <sys/time.h> /* µsec def, gettimeofday() */
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> /* nsec def, nanosleep() */
#include <unistd.h>

#include <libHX/defs.h>
#include <libHX/option.h>
#include <libHX/string.h>
#include "dev.h"
#include "rpl_packet.h"
#include "lib.h"
#include "pctrl.h"
#include "rdsh.h"

/* Definitions */
#define MICROSECOND 1000000
#define NANOSECOND  1000000000

enum {
	/* current follow mode state: */
	FM_NONE = 0,
	/* playing old content */
	FM_CATCHUP,
	/* already hit EOF once and waiting for new data */
	FM_LIVE,
	/* same as FM_LIVE, but different code parts are executed */
	FM_NODELAY,
};

struct {
	double factor;
	long ovcorr, maxdelay, skval;
	unsigned int follow, nodelay, sktype, no_pctrl, showtime;
} Opt = {
	.factor   = 1.0,
	.follow   = false,
	.maxdelay = -1,
	.nodelay  = false,
	.sktype   = PCTRL_NONE,
	.skval    = false,
	.showtime = false,
	.no_pctrl = false,
};

/* Functions */
static int replay_file(int, const char *);
static void e_proc(int, struct rpldsk_packet *, struct pctrl_info *, char *,
	struct rpltime *, long *);

static unsigned long calc_ovcorr(unsigned long, int);
static bool find_next_packet(int, const struct pctrl_info *);
static bool get_options(int *, const char ***);
static void getopt_jump(const struct HXoptcb *);
static void getopt_msec(const struct HXoptcb *);
static void getopt_skip(const struct HXoptcb *);
static inline ssize_t read_nullfm(int, size_t);
static ssize_t read_waitfm(int, void *, size_t, const struct pctrl_info *);
static bool seek_to_end(int, const struct pctrl_info *);
static void usleep_ovcorr(struct timeval *, long *);
static void tv_delta(const struct rpltime *, const struct rpltime *,
	struct timeval *);
static inline unsigned long long tv2usec(const struct timeval *);
static inline void usec2tv(unsigned long long, struct timeval *);

//-----------------------------------------------------------------------------
int main(int argc, const char **argv)
{
	const char *auto_stdin[] = {"/dev/stdin", NULL};
	unsigned int i;

	load_locale(*argv);
	if (!get_options(&argc, &argv))
		return EXIT_FAILURE;

	fprintf(stderr, "> ttyreplay " TTYRPLD_VERSION "\n");
	fprintf(stderr, _(
	        "This program comes with ABSOLUTELY NO WARRANTY; it is free software and you\n"
	        "you are welcome to redistribute it under certain conditions; for details see\n"
	        "the \"LICENSE.GPL2\" file which should have come with this program.\n"
	));

	++argv;
	--argc;
	if (argc == 0) {
		if (!isatty(STDIN_FILENO)) {
			fprintf(stderr, _("No arguments given but STDIN seems "
			        "to be a pipe/file -- replaying from STDIN\n"));
			argv = auto_stdin;
		} else {
			fprintf(stderr, _("No filename(s) given.\n"));
			exit(EXIT_FAILURE);
		}
	}

	setvbuf(stdout, NULL, _IONBF, 0);
	Opt.ovcorr = calc_ovcorr(0, 100);
	if (!Opt.no_pctrl)
		pctrl_init();

	for (i = 0; i < argc; /* see switch() */) {
		const char *file = (argv[i][0] == '-' && argv[i][1] == '\0') ?
		                   "/dev/stdin" : argv[i];
		int fd, rv;

		if ((fd = open(file, O_RDONLY)) < 0) {
			fprintf(stderr, _("Could not open %s: %s\n"),
			        argv[i++], strerror(errno));
			continue;
		}

		rv = replay_file(fd, file);
		close(fd);

		switch (rv) {
		case PCTRL_PREV:
			if (i > 0)
				--i;
			break;
		case PCTRL_NEXT:
			if (i < argc - 1)
				++i;
			break;
		case PCTRL_EXIT:
			i = argc;
			break;
		default:
			++i;
			break;
		} /* switch */
	}

	pctrl_exit();
	return EXIT_SUCCESS;
}

static int replay_file(int fd, const char *name)
{
	struct rpldsk_packet packet;
	struct pctrl_info ps;
	struct rpltime stamp;
	ssize_t ret;
	long skew = 0;
	char tick = false;

	if (Opt.follow == FM_LIVE && !seek_to_end(fd, &ps))
		return 0;

	memset(&ps, 0, sizeof(struct pctrl_info));
	ps.echo   = 0;
	ps.factor = Opt.factor;
	ps.sktype = Opt.sktype;
	ps.skval  = Opt.skval;
	pctrl_activate(&ps);

	while ((ret = read_waitfm(fd, &packet,
	    sizeof(struct rpldsk_packet), &ps)) == sizeof(struct rpldsk_packet))
	{
		if (packet.magic != MAGIC_SIG) {
			fprintf(stderr, _("\n" "<Packet inconsistency! "
			        "Trying to find next valid packet.>\n"));
			tick = 0;
			/*
			 * If read() in find_next_packet() generates an error,
			 * it will be catched when the condition is
			 * re-evaulated upon continue.
			 */
			find_next_packet(fd, &ps);
			continue;
		}

		SWAB1(&packet.size);
		SWAB1(&packet.time.tv_sec);
		SWAB1(&packet.time.tv_usec);

		switch (packet.event) {
		case EVT_WRITE:
			e_proc(fd, &packet, &ps, &tick, &stamp, &skew);
			break;
		case EVT_ID_PROG:
			fprintf(stderr, "\e[1;37;41m%s ", _("Created using")); /* ] */
			read_through(fd, STDERR_FILENO, packet.size);
			fprintf(stderr, "\e[0m\n"); /* ] */
			break;
		case EVT_ID_DEVPATH:
			fprintf(stderr, "\e[1;37;41m%s: ", _("Device path")); /* ] */
			read_through(fd, STDERR_FILENO, packet.size);
			fprintf(stderr, "\e[0m\n"); /* ] */
			break;
		case EVT_ID_TIME:
			fprintf(stderr, "\e[1;37;41m%s ", _("Recorded on")); /* ] */
			read_through(fd, STDERR_FILENO, packet.size);
			fprintf(stderr, "\e[0m\n"); /* ] */
			break;
		case EVT_ID_USER:
			fprintf(stderr, "\e[1;37;41m%s: ", _("User")); /* ] */
			read_through(fd, STDERR_FILENO, packet.size);
			fprintf(stderr, "\e[0m\n"); /* ] */
			break;
		case EVT_LCLOSE:
			fprintf(stderr, "\n\e[1;37;42m%s\e[0m\n", /* ]] */
			        _("<tty device has been closed>"));
			read_nullfm(fd, packet.size);
			break;
		case EVT_READ:
			if (ps.echo) {
				printf("\e[1;37;45m"); /* ] */
				e_proc(fd, &packet, &ps, &tick, &stamp, &skew);
				printf("\e[0m"); /* ] */
				break;
			}
			/* fallthrough */
		default:
			/* default case: ignore unknown (may be new) packet types
			this version does not support (yet). */
			read_nullfm(fd, packet.size);
			break;
		} /* switch */

		if (ps.break_command != PCTRL_NONE)
			break;
	}

	pctrl_deactivate(0);

	if (ret < 0)
		fprintf(stderr, "\n\e[1;37;41m%s\e[0m\nread(): %s\n", /* ]] */
		        _("<Error while reading from stream>"),
		        strerror(errno));

	printf("\n\e[1;37;41m"); /* ] */
	printf(_("<Log replaying of %s finished>"), name);
	printf("\e[0m\n"); /* ] */
	return ps.break_command;
}

static void e_proc(int fd, struct rpldsk_packet *p, struct pctrl_info *i,
    char *tick, struct rpltime *stamp, long *skew)
{
	if (i->sktype == PCTRL_SKPACK && i->skval-- > 0) {
		/* Just skip, do not update tick/stamp/skew/delta */
		read_through(fd, STDOUT_FILENO, p->size);
		return;
	}

	if (!*tick) {
		/* No delay before the first packet has been replayed */
		*tick = true;
		if (i->skval > 0) {
			memcpy(stamp, &p->time, sizeof(*stamp));
			read_through(fd, STDOUT_FILENO, p->size);
			return;
		}
	} else if (Opt.follow != FM_LIVE && Opt.follow != FM_NODELAY) {
		/*
		 * ... only when we know the next packet (which is already read by now)
		 * the time difference can be calculated. This code is not
		 * executed in FM_LIVE mode, because we have been already
		 * waiting enough with read_wait().
		 */
		struct timeval delta;
		tv_delta(stamp, &p->time, &delta);

		if (i->sktype == PCTRL_SKTIME &&
		    (i->skval -= tv2usec(&delta) / 1000) > 0) {
			memcpy(stamp, &p->time, sizeof(*stamp));
			read_through(fd, STDOUT_FILENO, p->size);
			return;
		}

		if (i->factor != 1.0)
			/* -S switch for the fast */
			usec2tv(static_cast(double, tv2usec(&delta)) /
			        i->factor, &delta);

		if (Opt.maxdelay > 0 && tv2usec(&delta) > Opt.maxdelay)
			/* -m switch for those who do not want to wait */
			usec2tv(Opt.maxdelay, &delta);

		usleep_ovcorr(&delta, skew);
	}

	while (i->paused)
		usleep(100000);

	memcpy(stamp, &p->time, sizeof(*stamp));
	read_through(fd, STDOUT_FILENO, p->size);

	if (Opt.showtime) {
		struct tm tm;
		char buf[80];
		time_t now = p->time.tv_sec;

		localtime_r(&now, &tm);

		if (Opt.showtime == 1)
			strftime(buf, sizeof(buf), "\e7\e[1;300H\e[10D" /* ]] */
			         "\e[0;7m[\e[0;1;37;41m" "%H:%M:%S"
			         "\e[0;7m]\e8\e[0m", &tm); /* ]]]] */
		else
			strftime(buf, sizeof(buf), "\e7\e[1;300H\e[13D" /* ]] */
			         "\e[0;7m[\e[0;1;37;41m" "%d.%m %H:%M"
			         "\e[0;7m]\e8\e[0m", &tm); /* ]]]] */

		write(STDOUT_FILENO, buf, strlen(buf));
	}
	return;
}

//-----------------------------------------------------------------------------
static unsigned long calc_ovcorr(unsigned long ad, int rd)
{
	struct timespec s = {.tv_sec = 0, .tv_nsec = ad};
	struct timeval start, stop;
	unsigned long av = 0;
	int count = rd;

	fprintf(stderr, _("Calculating average overhead..."));

	while (count--) {
		gettimeofday(&start, NULL);
		nanosleep(&s, NULL);
		gettimeofday(&stop, NULL);
		av += MICROSECOND * (stop.tv_sec - start.tv_sec) +
		      stop.tv_usec - start.tv_usec;
	}

	av /= rd;
	fprintf(stderr, " %lu µs\n", av);
	return av;
}

static bool find_next_packet(int fd, const struct pctrl_info *ps)
{
	/* Interesting, there is more algorithmic code than simple
	code in ttyreplay. Outstanding. */
#define LZ           sizeof(struct rpldsk_packet)
#define BZ           (2 * LZ)
#define MAGIC_OFFSET offsetof(struct rpldsk_packet, magic)
	char buf[BZ];
	struct rpldsk_packet *packet = static_cast(void *, buf);
	size_t s;
	unsigned int ok = 0;

	if (read_waitfm(fd, buf, BZ, ps) < BZ)
		return false;

	while (true) {
		char *ptr;

		/*
		 * Indeed, the many read() calls get more and more data from
		 * the stream without displaying it. Anyway, if we found a way
		 * into this function, there is a reason to it.
		 */

		if ((ptr = memchr(buf, MAGIC_SIG, BZ)) != NULL &&
		    ptr - buf >= MAGIC_OFFSET) {
			/*
			 * A magic byte has been found and the packet start is
			 * complete
			 */
			char *ctx = reinterpret_cast(char *,
			            containerof(ptr, struct rpldsk_packet, magic));
			if (ctx != buf) {
				size_t cnt = buf + BZ - ctx;
				ctx = memmove(buf, ctx, cnt);
				if (read_waitfm(fd, buf + cnt, BZ - cnt, ps) < BZ - cnt)
					return false;
			}
		} else if (ptr != NULL) {
			/*
			 * Magic byte, but of no use. Discard it, and fill up
			 * with new data from the descriptor.
			 */
			size_t cnt = buf + BZ - ptr - 1;
			memmove(buf, ptr + 1, cnt);
			if (read_waitfm(fd, buf + cnt, BZ - cnt, ps) < BZ - cnt)
				return false;
			continue;
		} else {
			/*
			 * No magic byte, but since it might be just the next
			 * byte in the stream, only read LZ bytes.
			 */
			memmove(buf, buf + BZ - LZ, BZ - LZ);
			if (read_waitfm(fd, buf + LZ, BZ - LZ, ps) < BZ - LZ)
				return false;
			continue;
		}

		s = packet->size;
		if (s > 4096) {
			/*
			 * The default tty buffer size is 4096, and any size
			 * above this is questionable at all. Start with a new
			 * slate.
			 */
			ok = 0;
			memmove(buf, buf + LZ, BZ - LZ);
			if (read_waitfm(fd, buf, BZ, ps) < BZ)
				return false;
			continue;
		}

		if (s < LZ) {
			memmove(buf, buf + LZ + s, BZ - LZ - s);
			if (read_waitfm(fd, buf + BZ - LZ - s, LZ + s, ps) < LZ + s)
				return false;
		} else {
			/*
			 * There is no header (according to .size) in our buffer, so we
			 * can blindly munge lots of data.
			 */
			if (!read_nullfm(fd, s - LZ) || read_waitfm(fd, buf, BZ, ps) < BZ)
				return false;
		}

		if ((ptr = memchr(buf, MAGIC_SIG, MAGIC_OFFSET + 1)) == NULL ||
		    ptr - buf != MAGIC_OFFSET) {
			/*
			 * If the size field does not match up with the next
			 * magic byte, drop it all.
			 */
			ok = 0;
			continue;
		}

		if (++ok >= 2)
			break;
	}

	fprintf(stderr, _("\n" "<Found packet boundary>\n"));

	/* Finally adjust the read pointer to a packet boundary */
	if ((s = packet->size) < 16) {
		/*
		 * Mmhkay, there is another header other than the current
		 * (packet) in the buffer. Crap, another one gone.
		 */
		memmove(buf, buf + LZ + s, BZ - LZ - s);
		if (read_waitfm(fd, buf + BZ - LZ - s, s, ps) < s ||
		    !read_nullfm(fd, packet->size))
			return false;
	} else {
		/* Just subtract what we have already read into the buffer */
		if (!read_nullfm(fd, s - LZ))
			return false;
	}

	return true;
#undef BZ
#undef LZ
#undef MAGIC_OFFSET
}

static bool get_options(int *argc, const char ***argv)
{
    struct HXoption options_table[] = {
        {.ln = "no-pctrl", .type = HXTYPE_NONE, .ptr = &Opt.no_pctrl,
         .help = _("Do not enable playing controls")},
        {.sh = 'F', .type = HXTYPE_VAL, .ptr = &Opt.follow, .val = FM_LIVE,
         .help = _("Live feed follow mode (like `tail -f`)")},
        {.sh = 'J', .type = HXTYPE_STRING, .cb = getopt_jump,
         .help = _("Seek to time position (HH:MM:SS or total SEC, "
         "see docs)"), .htyp = "tspec"},
        {.sh = 'S', .type = HXTYPE_DOUBLE, .ptr = &Opt.factor,
         .help = _("Speed factor (default: 1.0)")},
        {.sh = 'T', .type = HXTYPE_VAL, .ptr = &Opt.follow, .val = FM_NODELAY,
         .help = _("No delays (useful for text file grepping)")},
        {.sh = 'f', .type = HXTYPE_VAL, .ptr = &Opt.follow, .val = FM_CATCHUP,
         .help = _("Catch-up follow mode (play file, switch to live feed on EOF)")},
        {.sh = 'j', .type = HXTYPE_LONG, .ptr = &Opt.skval, .cb = getopt_skip,
         .help = _("Skip the given number of EVT_WRITE packets")},
        {.sh = 'm', .type = HXTYPE_LONG, .cb = getopt_msec,
         .help = _("Maximum delay to wait between packets"), .htyp = _("msec")},
        {.sh = 't', .type = HXTYPE_NONE | HXOPT_INC, .ptr = &Opt.showtime,
         .help = _("Show the timestamp at the top of the screen")},
        HXOPT_AUTOHELP,
        HXOPT_TABLEEND,
    };

    return HX_getopt(options_table, argc, argv, HXOPT_USAGEONERR) > 0;
}

static void getopt_jump(const struct HXoptcb *cbi)
{
	char *sec = "", *min = "", *hr = "";
	char *tmp = HX_strdup(cbi->data), *origtmp = tmp;

	Opt.sktype = PCTRL_SKTIME;
	Opt.skval  = 0;

	if ((sec = strrchr(tmp, ':')) == NULL) {
		sec = tmp;
	} else {
		*sec++ = '\0';
		if ((min = strrchr(tmp, ':')) == NULL) {
			min = tmp;
		} else {
			*min++ = '\0';
			hr = tmp;
		}
	}

	Opt.skval = (strtol(sec, NULL, 0) + 60 * strtol(min, NULL, 0) +
	            3600 * strtol(hr, NULL, 0)) * 1000;
	free(origtmp);
	return;
}

static void getopt_msec(const struct HXoptcb *cbi)
{
	Opt.maxdelay = cbi->data_long * 1000;
	return;
}

static void getopt_skip(const struct HXoptcb *cbi)
{
	Opt.sktype = PCTRL_SKPACK;
	return;
}

static inline ssize_t read_nullfm(int fd, size_t count)
{
	return G_skip(fd, count, Opt.follow != FM_NONE &&
	       Opt.follow != FM_NODELAY);
}

static ssize_t read_waitfm(int fd, void *buf, size_t count,
    const struct pctrl_info *ps)
{
	/*
	 * Wrapper function for either read() or read_wait(). If EOF has been
	 * detected (at this time), we switch into FM_LIVE mode (if currently
	 * in FM_CATCHUP mode).
	 */
	struct stat sb;
	off_t pos;

	if (Opt.follow == FM_NONE || Opt.follow == FM_NODELAY)
		/*
		 * If no follow mode is selected, the while() loop in
		 * replay_file() shall terminate as soon as we encounter EOF or
		 * have a short read (i.e.  read less then count).
		 */
		return read(fd, buf, count);

	/*
	 * If it is a live feed (follow mode), complete the read (i.e. read all
	 * requested bytes. fstat() must come after lseek() -- in the very case
	 * that the file size increases inbetween.
	 */
	if (Opt.follow == FM_CATCHUP && (pos = lseek(fd, 0, SEEK_CUR)) != 0 &&
	  fstat(fd, &sb) == 0 && pos == sb.st_size) {
		fprintf(stderr, "\n\e[1;37;41m%s\e[0m\n",
		        _("<Switching to live feed follow mode>"));
		Opt.follow = FM_LIVE;
	}

	return read_wait(fd, buf, count, ps);
}

static bool seek_to_end(int fd, const struct pctrl_info *ps)
{
	off_t a, b;

	a = lseek(fd, 0, SEEK_END);
	if (a >= 0) {
		while ((b = lseek(fd, 0, SEEK_END)) != a)
			a = b;
	} else if (errno == ESPIPE) {
		/*
		 * Some workaround for non-seekable descriptors. G_skip() is
		 * not suitable, since ... it's just not designed to do what is
		 * below, i.e.  non-blocking-until-EOF-call-it-something.
		 */
		char buf[4096];

		fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
		fprintf(stderr, _("Reading from something that does not "
		        "support seeking (a pipe?),\n"
		        "skipping to a position where reading would block\n"));

		while (read(fd, buf, sizeof(buf)) > 0)
			;
		if (errno != EAGAIN) {
			perror("read()");
			return false;
		}
		fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) & ~O_NONBLOCK);
	} else {
		return false;
	}

	return find_next_packet(fd, ps);
}

/*  usleep_ovcorr
 *  @req:       how long to pause
 *  @skew:      skew counter
 */
static void usleep_ovcorr(struct timeval *req, long *skew)
{
	/*
	 * The manual page for nanosleep(2) says this under the "BUGS" section:
	 *
	 * """The current implementation of nanosleep is based on the normal
	 *    kernel timer mechanism, which has a resolution of 1/HZ s."""
	 *
	 * (This depends on CONFIG_HZ.)
	 *
	 *  100 HZ = 10 ms = 10000 µs
	 *  250 HZ =  4 ms =  4000 µs
	 * 1000 HZ =  1 ms =  1000 µs
	 *
	 * """Therefore, nanosleep pauses always for at least the specified
	 *    time, however it can take up to 4 ms [sic: 1 tick] longer than
	 *    specified until the process becomes runnable again. [...]
	 *
	 *    As some applications require much more precise pauses (e.g., in
	 *    order to control some time-critical hardware), nanosleep is also
	 *    capable of short high-precision pauses. If the process is
	 *    scheduled under a real-time policy like SCHED_FIFO or SCHED_RR,
	 *    then pauses of up to 2 ms will be performed as busy waits with
	 *    microsecond precision."""
	 *
	 * Unfortunately, repeated busy waits lock out all other applications,
	 * so selecting another scheduling policy is inacceptable for
	 * ttyreplay, also because selecting another policy is only available
	 * to the superuser.
	 *
	 * Thus, I have designed a function that tries to cope with the longer
	 * delays by waiting a little shorter the next time when the previous
	 * call took longer. The minimal margin of course still is 1/250 s, why
	 * the algorithm splits up into multiple cases to make the best
	 * performance. They are explained below.
	 */

	struct timeval start, stop;
	struct timespec nano_req;
	long long req_usec;
	long dur, over;

	gettimeofday(&start, NULL);
	req_usec = tv2usec(req);

	if (req_usec > Opt.ovcorr) {
		if (*skew + Opt.ovcorr <= req_usec) {
			/*
			 * If the accumulated skew time plus the minimal delay
			 * fits into the request, the request is reduced and
			 * the skew is zeroed.
			 */
			req_usec -= *skew + Opt.ovcorr;
			usec2tv(req_usec, req);
			*skew = 0;
		} else {
			/*
			 * There is more skew than the current request, so we
			 * can take away at most the requested time. (In simple
			 * language: We already paused enough in the past so we
			 * can entirely skip this pause.)
			 */
			*skew -= req_usec;
			return;
		}
	} else if (*skew >= -Opt.ovcorr) {
		/*
		 * Specialty case: Allow the skew to become negative up to
		 * -1/HZ s (= having paused too few).
		 */
		*skew -= req_usec;
		return;
	}

	/*
	 * If none of these three cases, or case 2 applies, nanosleep() will be
	 * called with the time request.
	 */
	nano_req.tv_sec  = req->tv_sec;
	nano_req.tv_nsec = req->tv_usec * 1000;
	nanosleep(&nano_req, NULL);
	gettimeofday(&stop, NULL);

	/*
	 * Calculate the actual duration of nanosleep(), and from that, the
	 * overhead (actual time minus wanted time) which is added to the skew.
	 */
	dur    = MICROSECOND * (stop.tv_sec - start.tv_sec) +
	         (stop.tv_usec - start.tv_usec);
	over   = dur - req_usec;
	*skew += over;
	return;
}

/*
 *  tv_delta
 *  @past:      previous time
 *  @now:       current time
 *  @dest:      pointer to result
 *
 *  Calculates the time difference between @past and @now and stores the result
 *  in @dest. All parameters in µs.
 */
static void tv_delta(const struct rpltime *past, const struct rpltime *now,
    struct timeval *dest)
{
	unsigned long sec = now->tv_sec - past->tv_sec;
	long acc = now->tv_usec - past->tv_usec;

	if (acc < 0) {
		dest->tv_sec  = sec - 1;
		dest->tv_usec = MICROSECOND + acc;
	} else {
		dest->tv_sec  = sec;
		dest->tv_usec = acc;
	}

	return;
}

static inline unsigned long long tv2usec(const struct timeval *req)
{
	return req->tv_sec * MICROSECOND + req->tv_usec;
}

static inline void usec2tv(unsigned long long usec, struct timeval *req)
{
	req->tv_sec  = usec / MICROSECOND;
	req->tv_usec = usec % MICROSECOND;
	return;
}

//=============================================================================

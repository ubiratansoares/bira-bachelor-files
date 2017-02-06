/*
 *	ttyrpld/k_netbsd-4.0/rpldev.c
 *	Copyright © Jan Engelhardt <jengelh [at] medozas de>, 2004 - 2008
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions
 *	are met:
 *
 *	1. Redistributions of source code must retain the above copyright
 *	   notice, this list of conditions and the following disclaimer.
 *	2. Redistributions in binary form must reproduce the above copyright
 *	   notice, this list of conditions and the following disclaimer in the
 *	   documentation and/or other materials provided with the distribution.
 *	3. Redistributions of modified code that are made available only
 *	   in binary form require sending a description to the ttyrpld
 *	   project maintainer on what has been changed.
 *	4. Neither the names of the above-listed copyright holders nor the
 *	   names of any contributors may be used to endorse or promote
 *	   products derived from this software without specific prior
 *	   written permission.
 *
 *	NO WARRANTY. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *	CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 *	BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND
 *	FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *	COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY, OR
 *	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 *	BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *	ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 */
#include <sys/param.h>
#include <sys/conf.h>
#include <sys/endian.h>
#include <sys/errno.h>
#include <sys/ioccom.h>
#include <sys/lock.h>
#include <sys/malloc.h>
#include <sys/poll.h>
#include <sys/proc.h>
#include <sys/syslog.h>
#include <sys/systm.h>
#include <sys/time.h>
#include <sys/tty.h>
#include <sys/types.h>
#include <sys/vnode.h>
#include <sys/lkm.h>
#include <sys/rpldhk.h>
#include "../include/rpl_ioctl.h"
#include "../include/rpl_packet.h"

#define TTY_DEVNR(tty) htole32(mkdev_26(major((tty)->t_dev), \
                       minor((tty)->t_dev)))

/* Module stuff */
extern int rpldev_mod_lkmentry(struct lkm_table *, int, int);
static int kmd_event(struct lkm_table *, int);

/* Stage 2 functions */
static int rpldhc_open(const struct tty *);
static int rpldhc_read(const char *, size_t, const struct tty *);
static int rpldhc_write(const char *, size_t, const struct tty *);
static int rpldhc_lclose(const struct tty *);

/* Stage 3 functions */
static int rpldev_open(dev_t, int, int, struct lwp *);
static int rpldev_read(dev_t, struct uio *, int);
static int rpldev_ioctl(dev_t, u_long, caddr_t, int, struct lwp *);
static int rpldev_poll(dev_t, int, struct lwp *);
static int rpldev_close(dev_t, int, int, struct lwp *);

/* Functions */
static inline size_t avail_R(void);
static inline size_t avail_W(void);
static inline int circular_get(struct uio *, size_t);
static inline void circular_put(const void *, size_t);
static int circular_put_packet(struct rpldev_packet *, const void *, size_t);
static inline void fill_time(struct rpltime *);
static inline unsigned int min_uint(unsigned int, unsigned int);
static inline uint32_t mkdev_26(unsigned long, unsigned long);

/* Variables */
static struct lock Buffer_lock, Open_lock;
static char *Buffer, *BufRP, *BufWP;
static size_t Bufsize = 32 * 1024;
static unsigned int Open_count;

/* Module stuff */
static int kmi_usecount = 0;
static struct cdevsw kmi_fops = {
	.d_open     = rpldev_open,
	.d_close    = rpldev_close,
	.d_read     = rpldev_read,
	.d_write    = nowrite,
	.d_ioctl    = rpldev_ioctl,
	.d_stop     = nostop,
	.d_tty      = notty,
	.d_poll     = rpldev_poll,
	.d_mmap     = nommap,
	.d_kqfilter = nokqfilter,
	.d_type     = 0,
};

MOD_DEV("rpldev", "rpl", NULL, -1, &kmi_fops, 228);

//-----------------------------------------------------------------------------
int rpldev_mod_lkmentry(struct lkm_table *table, int cmd, int ver)
{
	LKM_DISPATCH(table, cmd, NULL, kmd_event, kmd_event, kmd_event);
}

static int kmd_event(struct lkm_table *table, int cmd)
{
	switch (cmd) {
		case LKM_E_LOAD:
			if (lkmexists(table))
				return EEXIST;
			lockinit(&Buffer_lock, PLOCK, "rpldev", 0, 0);
			lockinit(&Open_lock, PLOCK, "rpldev", 0, 0);
			break;
		case LKM_E_UNLOAD:
			if (kmi_usecount || Open_count > 0)
				return EBUSY;
			break;
		case LKM_E_STAT:
			break;
		default:
			return EIO;
	}
	return 0;
}

//-----------------------------------------------------------------------------
static int rpldhc_open(const struct tty *tty)
{
	struct rpldev_packet p;

	p.dev   = TTY_DEVNR(tty);
	p.size  = 0;
	p.event = EVT_OPEN;
	p.magic = MAGIC_SIG;
	fill_time(&p.time);
	return circular_put_packet(&p, NULL, 0);
}

static int rpldhc_read(const char *buf, size_t count, const struct tty *tty)
{
	struct rpldev_packet p;

	if (count == 0)
		return 0;

	p.dev   = TTY_DEVNR(tty);
	p.size  = htole16(count);
	p.event = EVT_READ;
	p.magic = MAGIC_SIG;
	fill_time(&p.time);
	return circular_put_packet(&p, buf, count);
}

static int rpldhc_write(const char *buf, size_t count, const struct tty *tty)
{
	struct rpldev_packet p;

	if (count == 0)
		return 0;

	p.dev   = TTY_DEVNR(tty);
	p.size  = htole16(count);
	p.event = EVT_WRITE;
	p.magic = MAGIC_SIG;
	fill_time(&p.time);
	return circular_put_packet(&p, buf, count);
}

static int rpldhc_lclose(const struct tty *tty)
{
	struct rpldev_packet p;

	p.dev   = TTY_DEVNR(tty);
	p.size  = 0;
	p.event = EVT_LCLOSE;
	p.magic = MAGIC_SIG;
	fill_time(&p.time);
	return circular_put_packet(&p, NULL, 0);
}

//-----------------------------------------------------------------------------
static int rpldev_open(dev_t dev, int flag, int mode, struct lwp *th)
{
	lockmgr(&Open_lock, LK_EXCLUSIVE, NULL);
	if (Open_count) {
		lockmgr(&Open_lock, LK_RELEASE, NULL);
		return EBUSY;
	}
	++kmi_usecount;
	++Open_count;
	lockmgr(&Open_lock, LK_RELEASE, NULL);

	if ((Buffer = malloc(Bufsize, M_TTYS, M_WAITOK | M_CANFAIL)) == NULL) {
		--kmi_usecount;
		--Open_count;
		return ENOMEM;
	}

	BufRP = BufWP = Buffer;
	rpl_open   = rpldhc_open;
	rpl_read   = rpldhc_read;
	rpl_write  = rpldhc_write;
	rpl_lclose = rpldhc_lclose;
	return 0;
}

static int rpldev_read(dev_t dev, struct uio *uio, int flags)
{
	size_t count;
	int ret = 0;

	lockmgr(&Buffer_lock, LK_EXCLUSIVE, NULL);
	if (Buffer == NULL)
		goto out;

	while (BufRP == BufWP) {
		lockmgr(&Buffer_lock, LK_RELEASE, NULL);
		if (flags & IO_NDELAY)
			return EWOULDBLOCK;
		if ((ret = tsleep(&Buffer, PCATCH, "rpldev", 0)) != 0)
			return ret;
		ret = 0;
		lockmgr(&Buffer_lock, LK_EXCLUSIVE, NULL);
		if (Buffer == NULL)
			goto out;
	}

	count = min_uint(uio->uio_resid, avail_R());
	ret   = circular_get(uio, count);
 out:
	lockmgr(&Buffer_lock, LK_RELEASE, NULL);
	return ret;
}

static int rpldev_ioctl(dev_t dev, u_long cmd, caddr_t data, int flags,
    struct lwp *th)
{
	size_t *ptr = (void *)data;
	int ret = 0;

	if (IOCGROUP(cmd) != RPL_IOC_MAGIC)
		return ENOTTY;
	if (ptr == NULL)
		return EFAULT;

	switch (cmd) {
		case RPL_IOC_GETBUFSIZE:
			*ptr = Bufsize;
			return 0;
		case RPL_IOC_GETRAVAIL:
			lockmgr(&Buffer_lock, LK_EXCLUSIVE, NULL);
			if (Buffer == NULL)
				goto out;
			*ptr = avail_R();
			lockmgr(&Buffer_lock, LK_RELEASE, NULL);
			return 0;
		case RPL_IOC_GETWAVAIL:
			lockmgr(&Buffer_lock, LK_EXCLUSIVE, NULL);
			if (Buffer == NULL)
				goto out;
			*ptr = avail_W();
			lockmgr(&Buffer_lock, LK_RELEASE, NULL);
			return 0;
		case RPL_IOC_IDENTIFY:
			*ptr = 0xC0FFEE;
			return 0;
		case RPL_IOC_SEEK:
			lockmgr(&Buffer_lock, LK_EXCLUSIVE, NULL);
			BufRP = Buffer + (BufRP - Buffer +
			        min_uint(*ptr, avail_R())) % Bufsize;
			lockmgr(&Buffer_lock, LK_RELEASE, NULL);
			return 0;
		case RPL_IOC_FLUSH:
			lockmgr(&Buffer_lock, LK_EXCLUSIVE, NULL);
			BufRP = BufWP;
			lockmgr(&Buffer_lock, LK_RELEASE, NULL);
			return 0;
	}

	ret = ENOTTY;
 out:
	return ret;
}

static int rpldev_poll(dev_t dev, int events, struct lwp *th)
{
	return (BufRP == BufWP) ? 0 : (POLLIN | POLLRDNORM);
}

static int rpldev_close(dev_t dev, int flag, int mode, struct lwp *th)
{
	rpl_open   = NULL;
	rpl_read   = NULL;
	rpl_write  = NULL;
	rpl_lclose = NULL;

	lockmgr(&Buffer_lock, LK_EXCLUSIVE, NULL);
	free(Buffer, M_TTYS);
	Buffer = NULL;
	lockmgr(&Buffer_lock, LK_RELEASE, NULL);
	--kmi_usecount;
	--Open_count;
	return 0;
}

//-----------------------------------------------------------------------------
static inline size_t avail_R(void)
{
	if (BufWP >= BufRP)
		return BufWP - BufRP;
	return BufWP + Bufsize - BufRP;
}

static inline size_t avail_W(void)
{
	if (BufWP >= BufRP)
		return BufRP + Bufsize - BufWP - 1;
	return BufRP - BufWP - 1;
}

static inline int circular_get(struct uio *uio, size_t count)
{
	size_t x = Buffer + Bufsize - BufRP;
	int ret;

	if (count < x) {
		if ((ret = uiomove(BufRP, count, uio)) != 0)
			return ret;
		BufRP += count;
	} else {
		if ((ret = uiomove(BufRP, x, uio)) != 0 ||
		    (ret = uiomove(Buffer, count - x, uio)) != 0)
			return ret;
		BufRP = Buffer + count - x;
	}

	return ret;
}

static inline void circular_put(const void *src, size_t count)
{
	size_t x = Buffer + Bufsize - BufWP;

	if (count < x) {
		memcpy(BufWP, src, count);
		BufWP += count;
	} else {
		memcpy(BufWP, src, x);
		memcpy(Buffer, src + x, count - x);
		BufWP = Buffer + count - x;
	}

	return;
}

static int circular_put_packet(struct rpldev_packet *p, const void *buf,
    size_t count)
{
	if (count > (size_t)(-sizeof(struct rpldev_packet) - 1))
		return ENOSPC;
	lockmgr(&Buffer_lock, LK_EXCLUSIVE, NULL);
	if (Buffer == NULL) {
		lockmgr(&Buffer_lock, LK_RELEASE, NULL);
		return 0;
	}
	if (avail_W() < sizeof(struct rpldev_packet) + count) {
		lockmgr(&Buffer_lock, LK_RELEASE, NULL);
		return ENOSPC;
	}

	circular_put(p, sizeof(struct rpldev_packet));
	if (count > 0)
		circular_put(buf, count);
	lockmgr(&Buffer_lock, LK_RELEASE, NULL);
	wakeup(&Buffer);
	return count;
}

static inline void fill_time(struct rpltime *x)
{
	struct timeval tv;
	microtime(&tv);

	if (sizeof(tv.tv_sec) == sizeof(uint32_t))
		x->tv_sec = htole32(tv.tv_sec);
	else if (sizeof(tv.tv_sec) == sizeof(uint64_t))
		x->tv_sec = htole64(tv.tv_sec);

	if (sizeof(tv.tv_usec) == sizeof(uint32_t))
		x->tv_usec = htole32(tv.tv_usec);
	else if (sizeof(tv.tv_usec) == sizeof(uint64_t))
		x->tv_usec = htole64(tv.tv_usec);

	return;
}

static inline unsigned int min_uint(unsigned int a, unsigned int b)
{
	return (a < b) ? a : b;
}

static inline uint32_t mkdev_26(unsigned long maj, unsigned long min)
{
	return (maj << 20) | (min & 0xFFFFF);
}

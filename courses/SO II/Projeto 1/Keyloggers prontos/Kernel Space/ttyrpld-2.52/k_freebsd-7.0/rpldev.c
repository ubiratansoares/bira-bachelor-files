/*
 *	ttyrpld/k_freebsd-6.2/rpldev.c
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
#include <sys/kernel.h>
#include <sys/malloc.h>
#include <sys/module.h>
#include <sys/mutex.h>
#include <sys/proc.h>
#include <sys/syslog.h>
#include <sys/systm.h>
#include <sys/time.h>
#include <sys/tty.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/rpldhk.h>
#include "../include/rpl_ioctl.h"
#include "../include/rpl_packet.h"

#define IO_NDELAY 0x10 /* from sys/vnode.h -- inclusion problems */

#if __FreeBSD_version >= 600000
#	define TTY_DEVNR(tty) \
		htole32(mkdev_26(umajor(dev2udev((tty)->t_dev)), \
		        uminor(dev2udev((tty)->t_dev))))
#else
#	define TTY_DEVNR(tty) htole32(mkdev_26(major((tty)->t_dev), \
	                      minor((tty)->t_dev)))
#endif

/* Module stuff */
static int kmd_event(module_t, int, void *);
static int kmd_load(void);
static int kmd_unload(void);

/* Stage 2 functions */
static int rpldhc_open(const struct tty *);
static int rpldhc_read(const char *, size_t, const struct tty *);
static int rpldhc_write(const char *, size_t, const struct tty *);
static int rpldhc_lclose(const struct tty *);

/* Stage 3 functions */
static int rpldev_open(struct cdev *, int, int, struct thread *);
static int rpldev_read(struct cdev *, struct uio *, int);
static int rpldev_ioctl(struct cdev *, u_long, caddr_t, int, struct thread *);
//static int rpldev_poll()
static int rpldev_close(struct cdev *, int, int, struct thread *);

/* Local functions */
static inline size_t avail_R(void);
static inline size_t avail_W(void);
static inline int circular_get(struct uio *, size_t);
static inline void circular_put(const void *, size_t);
static int circular_put_packet(struct rpldev_packet *, const void *, size_t);
static inline void fill_time(struct rpltime *);
static inline unsigned int min_uint(unsigned int, unsigned int);
static inline uint32_t mkdev_26(unsigned long, unsigned long);

/* Variables */
static MALLOC_DEFINE(Buffer_malloc, "rpldev", "rpldev ring buffer");
static struct mtx Buffer_lock, Open_lock;
static char *Buffer, *BufRP, *BufWP;
static size_t Bufsize = 32 * 1024;
static unsigned int Open_count;

/* Kernel module info stuff */
static int kmi_usecount;
static struct cdev *kmi_node;
static struct cdevsw kmi_fops = {
	.d_version = D_VERSION,
	.d_name    = "rpldev",
#if __FreeBSD_version < 600000
	.d_maj     = MAJOR_AUTO,
#endif
	.d_open    = rpldev_open,
	.d_read    = rpldev_read,
	.d_ioctl   = rpldev_ioctl,
	.d_close   = rpldev_close,
//	.d_poll    = rpldev_poll,
};
static moduledata_t kmi_rpldev = {
	.name   = "rpldev",
	.evhand = kmd_event,
};

DECLARE_MODULE(rpldev, kmi_rpldev, SI_SUB_DRIVERS, SI_ORDER_MIDDLE);

//-----------------------------------------------------------------------------
static int kmd_event(module_t mp, int type, void *data)
{
	int ret;

	switch (type) {
		case MOD_LOAD:
			if ((ret = kmd_load()) != 0)
				kmd_unload();
			return ret;
		case MOD_UNLOAD:
			return kmd_unload();
	}
	return EOPNOTSUPP;
}

static int kmd_load(void)
{
	mtx_init(&Buffer_lock, "rpldev", NULL, MTX_DEF);
	mtx_init(&Open_lock, "rpldev", NULL, MTX_DEF);
	kmi_node = make_dev(&kmi_fops, 0, UID_ROOT, 0, 0600, "rpl");
	if (kmi_node == NULL)
		return ENOMEM;
	return 0;
}

static int kmd_unload(void)
{
	if (kmi_usecount || Open_count > 0)
		return EBUSY;
	if (kmi_node != NULL)
		destroy_dev(kmi_node);
	mtx_destroy(&Buffer_lock);
	mtx_destroy(&Open_lock);
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

	if (tty == NULL || count == 0)
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

	if (tty == NULL || count == 0)
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
static int rpldev_open(struct cdev *cd, int flag, int mode,
    struct thread *th)
{
	mtx_lock(&Open_lock);
	if (Open_count) {
		mtx_unlock(&Open_lock);
		return EBUSY;
	}
	++kmi_usecount; // it's not perfect - Linux is better doing this
	++Open_count;
	mtx_unlock(&Open_lock);

	if ((Buffer = malloc(Bufsize, Buffer_malloc, M_WAITOK)) == NULL) {
		mtx_unlock(&Buffer_lock);
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

static int rpldev_read(struct cdev *cd, struct uio *uio, int flags)
{
	size_t count;
	int ret = 0;

	mtx_lock(&Buffer_lock);
	if (Buffer == NULL)
		goto out;

	while (BufRP == BufWP) {
		mtx_unlock(&Buffer_lock);
		if (flags & IO_NDELAY)
			return EWOULDBLOCK;
		if ((ret = tsleep(&Buffer, PCATCH, "rpldev", 0)) != 0)
			return ret;
		ret = 0;
		mtx_lock(&Buffer_lock);
		if (Buffer == NULL)
			goto out;
	}

	count = min_uint(uio->uio_resid, avail_R());
	ret   = circular_get(uio, count);
 out:
	mtx_unlock(&Buffer_lock);
	return ret;
}

static int rpldev_ioctl(struct cdev *cd, u_long cmd, caddr_t data, int flags,
    struct thread *th)
{
	/*
	 * Linux does ioctls better: negative non-zero is error, and the rest
	 * is some sort of success. Not so with BSD. Now I need to fiddle with
	 * an extra pointer (data) to put a return value in.
	 */
	size_t *ptr = (void *)data;
	int ret = 0;

	if (IOCGROUP(cmd) != RPL_IOC_MAGIC)
		return ENOTTY;
	if (ptr == NULL)
		return EFAULT;

	switch (cmd) {
		case RPL_IOC_GETBUFSIZE:
			/*
			 * This is BSD. No put_user() like in Linux.
			 * At least it seems so.
			 */
			*ptr = Bufsize;
			return 0;
		case RPL_IOC_GETRAVAIL:
			mtx_lock(&Buffer_lock);
			if (Buffer == NULL)
				goto out;
			*ptr = avail_R();
			mtx_unlock(&Buffer_lock);
			return 0;
		case RPL_IOC_GETWAVAIL:
			mtx_lock(&Buffer_lock);
			if (Buffer == NULL)
				goto out;
			*ptr = avail_W();
			mtx_unlock(&Buffer_lock);
			return 0;
		case RPL_IOC_IDENTIFY:
			*ptr = 0xC0FFEE;
			return 0;
		case RPL_IOC_SEEK:
			mtx_lock(&Buffer_lock);
			BufRP = Buffer + (BufRP - Buffer +
			        min_uint(*ptr, avail_R())) % Bufsize;
			mtx_unlock(&Buffer_lock);
			return 0;
		case RPL_IOC_FLUSH:
			mtx_lock(&Buffer_lock);
			BufRP = BufWP;
			mtx_unlock(&Buffer_lock);
			return 0;
	}

	ret = ENOTTY;
 out:
	return ret;
}

static int rpldev_close(struct cdev *cd, int flags, int fmt,
    struct thread *th)
{
	rpl_open   = NULL;
	rpl_read   = NULL;
	rpl_write  = NULL;
	rpl_lclose = NULL;

	mtx_lock(&Buffer_lock);
	free(Buffer, Buffer_malloc);
	Buffer = NULL;
	mtx_unlock(&Buffer_lock);
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

	return 0;
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
	mtx_lock(&Buffer_lock);
	if (Buffer == NULL) {
		mtx_unlock(&Buffer_lock);
		return 0;
	}
	if (avail_W() < sizeof(struct rpldev_packet) + count) {
		mtx_unlock(&Buffer_lock);
		return ENOSPC;
	}

	circular_put(p, sizeof(struct rpldev_packet));
	if (count > 0)
		circular_put(buf, count);
	mtx_unlock(&Buffer_lock);
	wakeup_one(&Buffer);
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

//=============================================================================

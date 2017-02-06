#ifndef TTYRPLD_IOCTL_H
#define TTYRPLD_IOCTL_H 1

#if defined(__linux__) && defined(__KERNEL__)
#	include <linux/ioctl.h>
#	include <linux/types.h>
#elif defined(__FreeBSD__) && defined(_KERNEL)
#	include <sys/ioccom.h>
#elif defined(__sun__) /* SunOS kernel-/userspace */
#	include <sys/ioccom.h>
#else /* userspace */
#	include <sys/types.h>
#	include <sys/ioctl.h>
#endif

enum {
	/*
	 * Some weird *BSD scheme makes me use IOWR, otherwise readin the
	 * pointer within kernel space does not seem to work.
	 */
	RPL_IOC_MAGIC      = 0xB7,
	RPL_IOC_GETBUFSIZE = _IOWR(RPL_IOC_MAGIC, 1, size_t),
	RPL_IOC_GETRAVAIL  = _IOWR(RPL_IOC_MAGIC, 2, size_t),
	RPL_IOC_GETWAVAIL  = _IOWR(RPL_IOC_MAGIC, 3, size_t),
	RPL_IOC_IDENTIFY   = _IOWR(RPL_IOC_MAGIC, 4, size_t),
	RPL_IOC_SEEK       = _IOWR(RPL_IOC_MAGIC, 5, size_t),
	RPL_IOC_FLUSH      = _IOWR(RPL_IOC_MAGIC, 6, size_t),
	RPL_IOC__MAXNUM    = _IOWR(RPL_IOC_MAGIC, 7, size_t),
};

#endif /* TTYRPLD_IOCTL_H */

/*
 *	ttyrpld/user/dev.h
 *	Copyright © CC Computer Consultants GmbH, 2004 - 2007
 *	Contact: Jan Engelhardt <jengelh [at] computergmbh de>
 *
 *	This file is part of ttyrpld. ttyrpld is free software; you can
 *	redistribute it and/or modify it under the terms of the GNU
 *	Lesser General Public License as published by the Free Software
 *	Foundation; either version 2 or 3 of the License.
 */
#ifndef TTYRPLD_DEV_H
#define TTYRPLD_DEV_H 1

#include "rdsh.h"

#define K26_MINORBITS 20
#define K26_MINORMASK 0xFFFFF
#define K26_MAJOR(dev) static_cast(unsigned long, (dev) >> K26_MINORBITS)
#define K26_MINOR(dev) static_cast(unsigned long, (dev) & K26_MINORMASK)
#define K26_MKDEV(major, minor) \
	(((major) << K26_MINORBITS) | ((minor) & K26_MINORMASK))
#define KD26_PARTS(dev) K26_MAJOR(dev), K26_MINOR(dev)

/* FreeBSD and OpenBSD
 * Scheme is:
 *   bits  0 - 7      (8 bits)    minor, lower part
 *   bits  8 - 15     (8 bits)    major
 *   bits 16 - 31    (16 bits)    minor, upper part
 */
#if defined(__FreeBSD__) || defined(__OpenBSD__)
#	define COMPAT_MAJOR(dev) \
		static_cast(unsigned long, ((dev) & 0xFF00) >> 8)
#	define COMPAT_MINOR(dev) \
		static_cast(unsigned long, ((dev) & 0xFF) | \
			(((dev) & 0xFFFF0000) >> 8))
#	define COMPAT_MKDEV(major, minor) \
		static_cast(unsigned long, (((major) & 0xFF) << 8) | \
		((minor) & 0xFF) | (((minor) & 0xFFFF00) << 8))

/* Solaris
 * Scheme is:
 *   bits  0 - 17    (18 bits)    minor
 *   bits 18 - 32    (14 bits)    major
 */
#elif defined(__sun__)
#	define COMPAT_MAJOR(dev) \
		static_cast(unsigned long, ((dev) & 0xFFFC0000) >> 18)
#	define COMPAT_MINOR(dev) \
		static_cast(unsigned long, (dev) & 0x3FFFF)
#	define COMPAT_MKDEV(major, minor) \
		static_cast(unsigned long, (((major) & 0x3FFF) << 18) | \
		((minor) & 0x3FFFF))

/* Glibc-Linux and NetBSD
 * Scheme is:
 *   bits  0 -  7     (8 bits)    minor, lower part
 *   bits  8 - 19    (12 bits)    major
 *   bits 20 - 31    (12 bits)    minor, upper part
 */
#else
#	define COMPAT_MAJOR(dev) \
		static_cast(unsigned long, ((dev) & 0xFFF00) >> 8)
#	define COMPAT_MINOR(dev) \
		static_cast(unsigned long, ((dev) & 0xFF) | \
		(((dev) & 0xFFF00000) >> 12))
#	define COMPAT_MKDEV(major, minor) \
		static_cast(unsigned long, ((minor) & 0xFF) | \
		(((minor) & 0xFFF00) << 12) | (((major) & 0xFFF) << 8))
#endif

#endif /* TTYRPLD_DEV_H */

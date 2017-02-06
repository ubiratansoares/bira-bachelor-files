/*
 *	ttyrpld/user/compat.h
 *	Copyright © CC Computer Consultants GmbH, 2004 - 2007
 *	Contact: Jan Engelhardt <jengelh [at] computergmbh de>
 *
 *	This file is part of ttyrpld. ttyrpld is free software; you can
 *	redistribute it and/or modify it under the terms of the GNU
 *	Lesser General Public License as published by the Free Software
 *	Foundation; either version 2 or 3 of the License.
 */
#ifndef TTYRPLD_COMPAT_H
#define TTYRPLD_COMPAT_H 1

#include "config.h"

#if defined(HAVE_GETPWUID5)
static inline struct passwd *rpld_getpwnam(const char *user,
    struct passwd *buffer, char *resbuf, size_t buflen)
{
	struct passwd *ret;
	getpwnam_r(user, buffer, resbuf, buflen, &ret);
	return ret;
}

static inline struct passwd *rpld_getpwuid(uid_t uid, struct passwd *resbuf,
    char *buffer, size_t buflen)
{
	struct passwd *ret;
	getpwuid_r(uid, resbuf, buffer, buflen, &ret);
	return ret;
}
#elif defined(HAVE_GETPWUID4)
#	define rpld_getpwnam getpwnam_r
#	define rpld_getpwuid getpwuid_r
#else
static inline struct passwd *rpld_getpwnam(const char *user,
    struct passwd *buffer, char *resbuf, size_t buflen)
{
	return getpwnam(user);
}

static inline struct passwd *rpld_getpwuid(uid_t uid, struct passwd *resbuf,
    char *buffer, size_t buflen)
{
	return getpwuid(uid);
}
#endif

#endif /* TTYRPLD_COMPAT_H */

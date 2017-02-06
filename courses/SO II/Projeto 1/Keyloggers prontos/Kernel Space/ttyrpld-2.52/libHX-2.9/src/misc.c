/*
 *	libHX/other.c
 *	Copyright © Jan Engelhardt <jengelh [at] medozas de>, 1999 - 2009
 *
 *	This file is part of libHX. libHX is free software; you can
 *	redistribute it and/or modify it under the terms of the GNU
 *	Lesser General Public License as published by the Free Software
 *	Foundation; either version 2.1 or 3 of the License.
 */
#include <sys/stat.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if defined(_WIN32)
#	include <io.h>
#else
#	include <sys/types.h>
#	include <sys/wait.h>
#	include <unistd.h>
#endif
#include <libHX/ctype_helper.h>
#include <libHX/misc.h>
#include "internal.h"

EXPORT_SYMBOL int HX_ffs(unsigned long n)
{
	int s = 0;
	if (n == 0)
		return -1;
	while ((n >>= 1) >= 1)
		++s;
	return s;
}

EXPORT_SYMBOL int HX_fls(unsigned long n)
{
	int i;
	for (i = 31; i >= 0; --i)
		if (n & (1 << i))
			return i;
	return -1;
}

EXPORT_SYMBOL void HX_hexdump(FILE *fp, const void *vptr, unsigned int len)
{
	const unsigned char *ptr = vptr;
	unsigned int i, j;
	bool tty = isatty(fileno(fp));

	fprintf(fp, "Dumping %u bytes\n", len);
	for (i = 0; i < len / 16; ++i) {
		fprintf(fp, "%04x | ", i * 16);
		for (j = 0; j < 16; ++j)
			fprintf(fp, "%02x%c", *ptr++, (j == 7) ? '-' : ' ');
		ptr -= 16;
		fprintf(fp, "| ");
		for (j = 0; j < 16; ++j, ++ptr)
			if (HX_isprint(*ptr))
				fprintf(fp, "%c", *ptr);
			else if (tty)
				fprintf(fp, "\e[31m.\e[0m"); // ]]
			else
				fprintf(fp, ".");
		fprintf(fp, "\n");
	}
	fprintf(fp, "%04x | ", i * 16);
	len -= i * 16;
	for (i = 0; i < len; ++i)
		fprintf(fp, "%02x%c", ptr[i], (i == 7) ? '-' : ' ');
	for (; i < 16; ++i)
		fprintf(fp, "   ");
	fprintf(fp, "| ");
	for (i = 0; i < len; ++i)
		if (HX_isprint(ptr[i]))
			fprintf(fp, "%c", ptr[i]);
		else if (tty)
			fprintf(fp, "\e[31m.\e[0m"); // ]]
		else
			fprintf(fp, ".");
	fprintf(fp, "\n");
}

EXPORT_SYMBOL long HX_time_compare(const struct stat *a,
    const struct stat *b, char sel)
{
	long r;

#if defined(HAVE_STRUCT_STAT_ST_MTIMENSEC)
	if (sel == 'm')
		return ((r = a->st_mtime - b->st_mtime) != 0) ?
		       r : a->st_mtimensec - b->st_mtimensec;
#ifdef HAVE_STRUCT_STAT_ST_OTIMENSEC
	else if (sel == 'o')
		return ((r = a->st_otime - b->st_otime) != 0) ?
		       r : a->st_otimensec - b->st_otimensec;
#endif
	else if (sel == 'a')
		return ((r = a->st_atime - b->st_atime) != 0) ?
		       r : a->st_atimensec - b->st_atimensec;
	else if (sel == 'c')
		return ((r = a->st_ctime - b->st_ctime) != 0) ?
		       r : a->st_ctimensec - b->st_ctimensec;
#elif defined(HAVE_STRUCT_STAT_ST_MTIM)
	if (sel == 'm')
		return ((r = a->st_mtim.tv_sec - b->st_mtim.tv_sec) != 0) ?
		       r : a->st_mtim.tv_nsec - b->st_mtim.tv_nsec;
#ifdef HAVE_STRUCT_STAT_ST_OTIM
	else if (sel == 'o')
		return ((r = a->st_otim.tv_sec - b->st_otim.tv_sec) != 0) ?
		       r : a->st_otim.tv_nsec - b->st_otim.tv_nsec;
#endif
	else if (sel == 'a')
		return ((r = a->st_atim.tv_sec - b->st_atim.tv_sec) != 0) ?
		       r : a->st_atim.tv_nsec - b->st_atim.tv_nsec;
	else if (sel == 'c')
		return ((r = a->st_ctim.tv_sec - b->st_ctim.tv_sec) != 0) ?
		       r : a->st_ctim.tv_nsec - b->st_ctim.tv_nsec;
#elif defined(HAVE_STRUCT_STAT_ST_MTIMESPEC)
	if (sel == 'm')
		return ((r = a->st_mtimespec.tv_sec - b->st_mtimespec.tv_sec) != 0) ?
		       r : a->st_mtimespec.tv_nsec - b->st_mtimespec.tv_nsec;
#ifdef HAVE_STRUCT_STAT_ST_OTIMESPEC
	else if (sel == 'o')
		return ((r = a->st_otimespec.tv_sec - b->st_otimespec.tv_sec) != 0) ?
		       r : a->st_otimespec.tv_nsec - b->st_otimespec.tv_nsec;
#endif
	else if (sel == 'a')
		return ((r = a->st_atimespec.tv_sec - b->st_atimespec.tv_sec) != 0) ?
		       r : a->st_atimespec.tv_nsec - b->st_atimespec.tv_nsec;
	else if (sel == 'c')
		return ((r = a->st_ctimespec.tv_sec - b->st_ctimespec.tv_sec) != 0) ?
		       r : a->st_ctimespec.tv_nsec - b->st_ctimespec.tv_nsec;
#elif defined(HAVE_STRUCT_STAT_ST_MTIME)
	if (sel == 'm')
		return a->st_mtime - b->st_mtime;
#ifdef HAVE_STRUCT_STAT_ST_OTIME
	else if (sel == 'o')
		return a->st_otime - b->st_otime;
#endif
	else if (sel == 'a')
		return a->st_atime - b->st_atime;
	else if (sel == 'c')
		return a->st_ctime - b->st_ctime;
#else
#	error Tis not ending well.
#endif
	return 0;
}

EXPORT_SYMBOL void HX_zvecfree(char **args)
{
	char **travp = args;
	while (*travp != NULL) {
		free(*travp);
		++travp;
	}
	free(args);
}

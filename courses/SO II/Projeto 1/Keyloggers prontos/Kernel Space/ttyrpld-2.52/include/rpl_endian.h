#ifndef TTYRPLD_ENDIAN_H
#define TTYRPLD_ENDIAN_H 1

#if defined(__FreeBSD__) || defined(__NetBSD__)
#	include <sys/endian.h>
#	ifndef __BYTE_ORDER
#		define __BYTE_ORDER	_BYTE_ORDER
#		define __LITTLE_ENDIAN _LITTLE_ENDIAN
#		define __BIG_ENDIAN	_BIG_ENDIAN
#	endif
#elif defined(__OpenBSD__)
#	include <machine/endian.h>
#	ifdef BYTE_ORDER
#		define __BYTE_ORDER	BYTE_ORDER
#		define __LITTLE_ENDIAN LITTLE_ENDIAN
#		define __BIG_ENDIAN	BIG_ENDIAN
#	endif
#elif defined(__sun__)
#	define __LITTLE_ENDIAN 1234
#	define __BIG_ENDIAN    4321
#	ifdef __sparc_
#		define __BYTE_ORDER __BIG_ENDIAN
#	else
#		define __BYTE_ORDER __LITTLE_ENDIAN
#	endif
#else
#	include <endian.h>
/* __BYTE_ORDER defined */
#endif

#if __BYTE_ORDER == __BIG_ENDIAN
#	define SWAB(ptr, sz) swab_be((ptr), (sz))
#	define SWAB1(ptr) swab_be((ptr), sizeof(*(ptr)))
#else
#	define SWAB(ptr, sz)
#	define SWAB1(ptr)
#endif

#endif /* RPL_ENDIAN_H */

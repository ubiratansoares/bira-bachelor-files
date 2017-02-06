#ifndef _LIBHX_MISC_H
#define _LIBHX_MISC_H 1

#ifndef __cplusplus
#	include <stdarg.h>
#	include <stdio.h>
#else
#	include <cstdarg>
#	include <cstdio>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct stat;

/*
 *	DIR.C
 */
enum {
	HXF_UID  = 1 << 0,
	HXF_GID  = 1 << 1,
	HXF_KEEP = 1 << 2,
};

extern void *HXdir_open(const char *);
extern const char *HXdir_read(void *);
extern void HXdir_close(void *);
extern int HX_copy_dir(const char *, const char *, unsigned int, ...);
extern int HX_copy_file(const char *, const char *, unsigned int, ...);
extern int HX_mkdir(const char *);
extern int HX_rrmdir(const char *);

/*
 *	DL.C
 */
extern void *HX_dlopen(const char *);
extern void *HX_dlsym(void *, const char *);
extern void HX_dlclose(void *);
extern const char *HX_dlerror(void);

/*
 *	MISC.C
 */
enum {
	HX_FSYSTEM_ARGV  = 1 << 0,
	HX_FSYSTEM_EXEC  = 1 << 1,
	HX_FSYSTEM_ARGV1 = 1 << 2,
};

extern int HX_ffs(unsigned long);
extern int HX_fls(unsigned long);
extern void HX_hexdump(FILE *, const void *, unsigned int);
extern long HX_time_compare(const struct stat *, const struct stat *, char);
extern void HX_zvecfree(char **);

/*
 *	RAND.C
 */
extern int HX_rand(void);
extern unsigned int HX_irand(unsigned int, unsigned int);

/*
 *	INLINE FUNCTIONS
 */
static inline int HX_zveclen(const char *const *args)
{
	int argk = 0;
	while (*args++ != NULL)
		++argk;
	return argk;
}

#ifdef __cplusplus
} /* extern "C" */

extern "C++" {

template<typename type> static inline type
HX_dlsym(void *handle, const char *symbol)
{
	return reinterpret_cast<type>(HX_dlsym(handle, symbol));
}

} /* extern "C++" */
#endif

#endif /* _LIBHX_MISC_H */

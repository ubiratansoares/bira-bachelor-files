#ifndef _RPLDHK_H
#define _RPLDHK_H 1

#ifdef _KERNEL

#include <sys/types.h>

extern int (*rpl_open)(const struct queue *);
extern int (*rpl_read)(const char *, size_t, const struct queue *);
extern int (*rpl_write)(const char *, size_t, const struct queue *);
extern int (*rpl_lclose)(const struct queue *);

#endif /* _KERNEL */

#endif /* _RPLDHK_H */

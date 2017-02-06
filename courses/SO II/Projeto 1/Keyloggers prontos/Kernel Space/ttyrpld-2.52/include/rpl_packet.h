#ifndef TTYRPLD_PACKET_H
#define TTYRPLD_PACKET_H 1

#define __PACKED __attribute__((packed))

#if defined(__linux__) && defined(__KERNEL__)
#	include <linux/time.h>
#	include <linux/types.h>
#elif (defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__sun)) && \
    defined(_KERNEL)
#	include <sys/time.h>
#	include <sys/types.h>
#else /* userspace */
#	include <sys/time.h>
#	include "rpl_stdint.h"
#endif

enum {
	EVT_NONE       = 0x00,
	EVT_OPEN       = 0x01,
	EVT_READ       = 0x02,
	EVT_WRITE      = 0x03,
	EVT_MAGIC      = 0x4A,
	EVT_LCLOSE     = 0x64,
	EVT_ID_PROG    = 0xF0,
	EVT_ID_DEVPATH = 0xF1,
	EVT_ID_TIME    = 0xF2,
	EVT_ID_USER    = 0xF3,
	EVT_max,
	MAGIC_SIG      = 0xEE,
};

struct rpltime {
	uint64_t tv_sec;
	uint32_t tv_usec;
};

struct rpldev_packet {
	uint32_t dev;
	uint16_t size;
	uint8_t event, magic;
	struct rpltime time;
} __PACKED;

struct rpldsk_packet {
	uint16_t size;
	uint8_t event, magic;
	struct rpltime time;
} __PACKED;

#endif /* TTYRPLD_PACKET_H */

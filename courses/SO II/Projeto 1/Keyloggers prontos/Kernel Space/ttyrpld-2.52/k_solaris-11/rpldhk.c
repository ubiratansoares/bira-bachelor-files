/*
 *	ttyrpld/k_solaris-11/rpldhk.c
 *	Copyright © CC Computer Consultants GmbH, 2006 - 2007
 *	Contact: Jan Engelhardt <jengelh [at] computergmbh de>
 *
 *	The contents of this file are subject to the terms of the Common
 *	Development and Distribution License, Version 1.0 only (the "License").
 *	You may not use this file except in compliance with the License.
 *
 *	You can obtain a copy of the license in the file "LICENSE.CDDL".
 *	See the License for the specific language governing permissions
 *	and limitations under the License.
 *
 *	When distributing Covered Code, include this CDDL HEADER in each
 *	file and include the License file.
 *	If applicable, add the following below this CDDL HEADER, with the
 *	fields enclosed by brackets "[]" replaced with your own identifying
 *	information: Portions Copyright [yyyy] [name of copyright owner]
 */
#include <sys/types.h>
#include <sys/ccompile.h>
#include <sys/cmn_err.h>
#include <sys/conf.h>
#include <sys/ddi.h>
#include <sys/mkdev.h>
#include <sys/modctl.h>
#include <sys/sunddi.h>
#include <sys/stream.h>
#include <sys/strsubr.h>
#include <sys/vnode.h>
#include "rpldhk.h"

/* Functions */
static int rpldhk_open(struct queue *, dev_t *, int, int, struct cred *);
static int rpldhk_read(struct queue *, struct msgb *);
static int rpldhk_write(struct queue *, struct msgb *);
static int rpldhk_lclose(struct queue *, int, struct cred *);
static int rpldhk_trap(void);
static void rpldhk_packet(struct queue *, struct msgb *, int);

/* Variables */
int (*rpl_open)(const struct queue *);
int (*rpl_read)(const char *, size_t, const struct queue *);
int (*rpl_write)(const char *, size_t, const struct queue *);
int (*rpl_lclose)(const struct queue *);

/* Module info */
static struct module_info rpldhk_minfo = {
	.mi_idnum  = 1337,
	.mi_idname = "rpldhk",
	.mi_minpsz = 0,
	.mi_maxpsz = INFPSZ,
	.mi_hiwat  = 2048,
	.mi_lowat  = 128,
};

static struct qinit rpldhk_rdinit = {
	.qi_qopen    = rpldhk_open,
	.qi_putp     = rpldhk_read,
	.qi_qclose   = rpldhk_lclose,
	.qi_minfo    = &rpldhk_minfo,
};

static struct qinit rpldhk_wrinit = {
	/*
	 * "The @open and @close procedures are only used on the read side of
	 * the queue and can be set to %NULL in the write-side @qinit stream
	 * stucture initialization." -STREAMS, p.101
	 */
	.qi_qopen    = rpldhk_trap,
	.qi_putp     = rpldhk_write,
	.qi_qclose   = rpldhk_trap,
	.qi_minfo    = &rpldhk_minfo,
};

static struct streamtab rpldhk_strops = {
	.st_rdinit = &rpldhk_rdinit,
	.st_wrinit = &rpldhk_wrinit,
};

static struct fmodsw rpldhk_devops = {
	.f_name = "rpldhk",
	.f_str  = &rpldhk_strops,
	/* Do I need D_NEW in ->f_flag? */
	.f_flag = D_MP,
};

static struct modlstrmod rpldhk_modldrv = {
	.strmod_modops   = &mod_strmodops,
	.strmod_linkinfo = "ttyrpld/2.18 rpldhk",
	.strmod_fmodsw   = &rpldhk_devops,
};

static struct modlinkage rpldhk_modlinkage = {
	.ml_rev     = MODREV_1,
	.ml_linkage = {&rpldhk_modldrv, NULL},
};

//-----------------------------------------------------------------------------
int _init(void)
{
	return mod_install(&rpldhk_modlinkage);
}

int _info(struct modinfo *mip)
{
	return mod_info(&rpldhk_modlinkage, mip);
}

int _fini(void)
{
	return mod_remove(&rpldhk_modlinkage);
}

//-----------------------------------------------------------------------------
static int rpldhk_open(struct queue *q, dev_t *dev, int oflag, int sflag,
    struct cred *cred)
{
	typeof(rpl_open) tmp = rpl_open;
	qprocson(q);
	if (tmp != NULL)
		tmp(q);
	return 0;
}

static int rpldhk_read(struct queue *q, struct msgb *mp)
{
	rpldhk_packet(q, mp, 0);
	putnext(q, mp);
	return 0;
}

static int rpldhk_write(struct queue *q, struct msgb *mp)
{
	rpldhk_packet(q, mp, 1);
	putnext(q, mp);
	return 0;
}

static int rpldhk_lclose(struct queue *q, int flag, struct cred *cred)
{
	typeof(rpl_lclose) tmp = rpl_lclose;
	qprocsoff(q);
	if (tmp)
		tmp(q);
	return 0;
}

static int rpldhk_trap(void)
{
	cmn_err(CE_NOTE, "This function should have never been called!\n");
	return 0;
}

static void rpldhk_packet(struct queue *q, struct msgb *mp, int wr)
{
	const struct msgb *mblk = mp;
	typeof(rpl_read)  tmpr  = rpl_read;
	typeof(rpl_write) tmpw  = rpl_write;

	for (mblk = mp; mblk != NULL; mblk = mblk->b_cont) {
		const struct datab *dblk = mblk->b_datap;
		if (mblk->b_rptr > mblk->b_wptr) {
			cmn_err(CE_NOTE, "Strange mblk received: r > w\n");
			continue;
		}

		if (dblk->db_type != M_DATA)
			continue;

		if (wr && tmpw != NULL)
			tmpw(mblk->b_rptr, mblk->b_wptr - mblk->b_rptr, q);
		else if (!wr && tmpr != NULL)
			tmpr(mblk->b_rptr, mblk->b_wptr - mblk->b_rptr, q);
	}
	return;
}

/*
 *	ttyrpld/user/pctrl.h
 *	Copyright © CC Computer Consultants GmbH, 2004 - 2007
 *	Contact: Jan Engelhardt <jengelh [at] computergmbh de>
 *
 *	This file is part of ttyrpld. ttyrpld is free software; you can
 *	redistribute it and/or modify it under the terms of the GNU
 *	Lesser General Public License as published by the Free Software
 *	Foundation; either version 2 or 3 of the License.
 */
#ifndef TTYRPLD_PCTRL_H
#define TTYRPLD_PCTRL_H 1

#include <stdbool.h>

enum {
	PCTRL_NONE = 0,
	PCTRL_PREV,
	PCTRL_NEXT,
	PCTRL_EXIT,
	PCTRL_SKPACK,
	PCTRL_SKTIME,
};

struct pctrl_info {
	double factor;
	long skval;
	unsigned int sktype, break_command;
	bool paused, echo;
};

extern int pctrl_init(void);
extern void pctrl_exit(void);
extern void pctrl_activate(struct pctrl_info *);
extern void pctrl_deactivate(int);

#endif /* TTYRPLD_PCTRL_H */

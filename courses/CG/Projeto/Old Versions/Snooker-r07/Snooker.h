#ifndef SNOOKER_H_INCLUDED
#define SNOOKER_H_INCLUDED

#include	<stdlib.h>
#include	<stdio.h>
#include	<math.h>
//#include	<conio.h>
#include	"SnookerDefs.h"
#include	"Table.h"
#include	"Ball.h"

typedef struct {
	PTable		table;
	unsigned	balls;
	PBall		*ball;
	int			paused;
} TSnooker, *PSnooker;

PSnooker	snookerCreate();
void		snookerDestroy(PSnooker *snkr);
void		snookerSetupTable(PSnooker snooker, TFloat totalWidth, TFloat totalHeight, TFloat width, TFloat height,
			TFloat pocketSize, TFloat atrCoef);
void		snookerSetupBalls(PSnooker snooker, unsigned balls, TFloat radius, TFloat mass);
void		snookerSetupWhiteBall(PSnooker snooker, TFloat radius, TFloat mass);
PBall		snookerGetWhiteBall(PSnooker snooker);
unsigned	snookerGetBalls(PSnooker snooker);
PBall		snookerGetBall(PSnooker snooker, unsigned ball);
void		snookerThink(PSnooker snooker, TFloat timeEllapsed);
void		snookerPaint(PSnooker snooker);
int			snookerIsPaused(PSnooker snooker);
void		snookerPause(PSnooker snooker, int pause);
void		snookerSetBallFlag(PSnooker snooker, int ball, unsigned flag);

#endif // SNOOKER_H_INCLUDED

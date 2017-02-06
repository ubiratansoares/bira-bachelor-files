#ifndef SNOOKER_H_INCLUDED
#define SNOOKER_H_INCLUDED

#include	<stdlib.h>
#include	<stdio.h>
#include	<math.h>
#include	"SnookerDefs.h"
#include	"Table.h"
#include	"Ball.h"
#include	"Texture.h"

typedef struct {
	int		mode, changed;
	TFloat	xc, yc, zc;										// camera position.
	TFloat	xv, yv, zv;										// camera looking at.
	TFloat	d;												// distance between position and looking at.
} TCamera, *PCamera;

typedef struct {
	TUVertex	position2D, dimension2D;					// 2D view port positioning and dimensioning (minimap).
	TUVertex	position3D, dimension3D;					// 3D view port positioning and dimensioning (3d scenario).
	PTable		table;
	unsigned	balls;
	PBall		*ball;
	int			paused;
	TCamera		camera;
	PTexture	wall, floor;
} TSnooker, *PSnooker;

PSnooker	snookerCreate();
void		snookerDestroy(PSnooker *snkr);
void		snookerSetupTable(PSnooker snooker, TFloat totalWidth, TFloat totalHeight, TFloat width, TFloat height,
			TFloat pocketSize, TFloat atrCoef);
int			snookerSetupOfficialBallColors(PSnooker snooker);
void		snookerSetTable2DBounds(PSnooker snooker, TUVertex p, TUVertex d);
void		snookerSetTable3DBounds(PSnooker snooker, TUVertex p, TUVertex d);
void		snookerSetupBalls(PSnooker snooker, unsigned balls, TFloat radius, TFloat mass);
void		snookerSetupCueBall(PSnooker snooker, TFloat radius, TFloat mass);
PBall		snookerGetCueBall(PSnooker snooker);
unsigned	snookerGetBalls(PSnooker snooker);
PBall		snookerGetBall(PSnooker snooker, unsigned ball);
void		snookerThink(PSnooker snooker, TFloat timeEllapsed);
void		snookerPaint2D(PSnooker snooker);
void		snookerPaint3D(PSnooker snooker);
int			snookerIsPaused(PSnooker snooker);
void		snookerPause(PSnooker snooker, int pause);
void		snookerClearBallFlag(PSnooker snooker, int ball, unsigned flag);
void		snookerSetBallFlag(PSnooker snooker, int ball, unsigned flag);
int			snookerMovementInProgress(PSnooker snooker);
int			snookerGameEnded(PSnooker snooker);
void		snookerTackleRandomBall(PSnooker snooker, TFloat speed);
void		snookerTackleRandom(PSnooker snooker, TFloat speed);
void		snookerTackleClosestBall(PSnooker snooker, TFloat speed);
void		snookerTackleFartestBall(PSnooker snooker, TFloat speed);
void		snookerTackle(PSnooker snooker, TFloat speed);
void		snookerSetCameraXC(PSnooker snooker, TFloat x);
void		snookerSetCameraYC(PSnooker snooker, TFloat y);
void		snookerSetCameraZC(PSnooker snooker, TFloat height);
void		snookerSetCameraXV(PSnooker snooker, TFloat x);
void		snookerSetCameraYV(PSnooker snooker, TFloat y);
void		snookerSetCameraZV(PSnooker snooker, TFloat z);
void		snookerSetCameraD(PSnooker snooker, TFloat distance);
void		snookerSetCameraAngle(PSnooker snooker, TFloat angle);
void		snookerAddCameraAngle(PSnooker snooker, TFloat angle);
void		snookerSetCameraMode(PSnooker snooker, int mode);
void		snookerAddCameraC(PSnooker snooker, TFloat x, TFloat y, TFloat z);
void		snookerCameraUpdate(PSnooker snooker);

#endif // SNOOKER_H_INCLUDED

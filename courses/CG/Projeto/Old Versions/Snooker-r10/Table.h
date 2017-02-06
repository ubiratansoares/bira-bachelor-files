#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

#include	"SnookerDefs.h"
#include	"SnookerUtils.h"
#include	"Ball.h"
#include	"Line.h"

#define		TABLE_BOUNDS	24

typedef struct {
	TFloat		width, height, totalWidth, totalHeight;
	TFloat		pocketSize, atrCoef;
	PLine		bounds[TABLE_BOUNDS];
	unsigned	pockets;
} TTable, *PTable;

PTable	tableCreate(TFloat totalWidth, TFloat totalHeight);
void	tableDestroy(PTable *table);
void	tableSetup(PTable table, TFloat width, TFloat height, TFloat pocketSize, TFloat atrCoef);
void	tablePaint(PTable t);
int		tableCollided(PTable, PBall);
int		tableCollision(PTable table, PBall ball, int hit);
TFloat	tableCollisionTime(PTable table, PBall ball, int hit);
TFloat	tableGetBorder(PTable table);
TFloat	tableGetAtrCoef(PTable table);
int		tableInPocket(PTable table, PBall ball, int hit);

#endif // TABLE_H_INCLUDED

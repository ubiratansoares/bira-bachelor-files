#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

#include	"SnookerDefs.h"
#include	"SnookerUtils.h"
#include	"Ball.h"
#include	"Line.h"
#include	"Texture.h"

#define		TABLE_BOUNDS	24
#define		TABLE_POCKETS	6

typedef struct {
	TFloat		width, height, totalWidth, totalHeight;
	TFloat		pocketSize, atrCoef;
	PLine		bound[TABLE_BOUNDS];
	unsigned	pockets;
	PVertex		pocket;
	PTexture	wood, field, steel;
} TTable, *PTable;

PTable	tableCreate(TFloat totalWidth, TFloat totalHeight);
void	tableDestroy(PTable *table);
void	tableSetup(PTable table, TFloat width, TFloat height, TFloat pocketSize, TFloat atrCoef);
void	tablePaint2D(PTable t);
void	tablePaint3D(PTable t, TFloat ballRadius);
int		tableCollided(PTable, PBall);
int		tableCollision(PTable table, PBall ball, int hit);
TFloat	tableCollisionTime(PTable table, PBall ball, int hit);
TFloat	tableGetBorder(PTable table);
TFloat	tableGetAtrCoef(PTable table);
int		tableInPocket(PTable table, PBall ball, int hit);
TFloat	tableGetTotalWidth(PTable table);
TFloat	tableGetTotalHeight(PTable table);

#endif // TABLE_H_INCLUDED

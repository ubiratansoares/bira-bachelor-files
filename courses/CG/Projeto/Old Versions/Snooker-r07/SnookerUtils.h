#ifndef SNOOKERUTILS_H_INCLUDED
#define SNOOKERUTILS_H_INCLUDED

#include "SnookerDefs.h"

typedef struct {
	TFloat	x, y;
} TVertex, *PVertex;

typedef struct {
	TVertex	p1, p2;
	TFloat	m;
} TLine, *PLine;

TFloat	sqr(TFloat x);
TVertex	vertex(TFloat x, TFloat y);
TFloat	distancePointToLine(TVertex p, PLine l);
void	drawCircle(TFloat radius, TFloat xc, TFloat yc);

#endif // SNOOKERUTILS_H_INCLUDED

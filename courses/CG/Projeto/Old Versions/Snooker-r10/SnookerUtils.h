#ifndef SNOOKERUTILS_H_INCLUDED
#define SNOOKERUTILS_H_INCLUDED

#include "SnookerDefs.h"

typedef struct {
	TFloat	x, y;
} TVertex, *PVertex;

typedef struct {
	TVertex	p1, p2;
} TLine, *PLine;

inline	TFloat	radToDegree(TFloat rad);
inline	TFloat	sqr(TFloat x);
inline	TVertex	vertex(TFloat x, TFloat y);
inline	int		sameVertex(TVertex v1, TVertex v2);
		TFloat	distancePointToPoint(TVertex p1, TVertex p2);
		TFloat	distancePointToLine(TVertex p, PLine l);
		TFloat	distancePointToLineSegment(TVertex p, PLine l);
inline	double	angularCoef(TVertex v1, TVertex v2);
		void	drawCircle(TFloat radius, TFloat xc, TFloat yc);

#endif // SNOOKERUTILS_H_INCLUDED

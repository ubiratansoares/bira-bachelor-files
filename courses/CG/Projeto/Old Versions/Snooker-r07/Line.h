#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include	"SnookerDefs.h"
#include	"SnookerUtils.h"
#include	"Ball.h"

PLine	lineCreate(TVertex p1, TVertex p2);
void	lineDestroy(PLine *line);
int		lineBallCollided(PLine line, PBall ball);

#endif // LINE_H_INCLUDED

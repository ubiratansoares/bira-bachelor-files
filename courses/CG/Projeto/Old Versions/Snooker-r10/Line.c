#include	"Line.h"

PLine lineCreate(TVertex p1, TVertex p2) {
	PLine	line = malloc(sizeof(TLine));
	line->p1 = p1;
	line->p2 = p2;
	return line;
}

void lineDestroy(PLine *line) {
	free(*line);
	*line = 0;
}

int lineBallCollided(PLine line, PBall ball) {
//	printf("distancePointToLineSegment()=%.3f\n",distancePointToLineSegment(vertex(ball->x,ball->y),line));
	return distancePointToLineSegment(vertex(ball->x,ball->y),line) <= ball->radius;
}

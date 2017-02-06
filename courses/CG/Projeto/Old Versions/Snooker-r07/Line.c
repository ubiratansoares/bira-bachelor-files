#include	"Line.h"

PLine lineCreate(TVertex p1, TVertex p2) {
	PLine	line = malloc(sizeof(TLine));
	line->p1 = p1;
	line->p2 = p2;
	line->m = (p2.y - p1.y) / (p2.x - p1.x);
	return line;
}

void lineDestroy(PLine *line) {
	free(*line);
	*line = 0;
}

int lineBallCollided(PLine line, PBall ball) {
//	printf("<<<<<<<<<<<<<<<<< distance=%.3f\n",distancePointToLine(vertex(ball->x,ball->y),line));
	return distancePointToLine(vertex(ball->x,ball->y),line) <= ball->radius;
}

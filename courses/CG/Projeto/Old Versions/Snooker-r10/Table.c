#include <stdlib.h>
#include <math.h>
#include <OpenGL/gl.h>
#include "Table.h"

PTable tableCreate(TFloat totalWidth, TFloat totalHeight) {
	PTable table = malloc(sizeof(TTable));
	table->totalWidth = totalWidth;
	table->totalHeight = totalHeight;
	return table;
}

void tableDestroy(PTable *table) {
	if ( table && *table ) {
		free(*table);
		*table = 0;
	}
}

void tableSetup(PTable t, TFloat width, TFloat height, TFloat pocketSize, TFloat atrCoef) {
	t->width = width;
	t->height = height;
	t->pocketSize = pocketSize;
	t->atrCoef = atrCoef;
	t->pockets = 0x22222;

	TFloat	slice = pocketSize / MSQRT_2;
	TFloat	recoil = slice;
//	TFloat	border = tableGetBorder(t);
	TFloat	border = 0.0;

	t->bounds[0] = lineCreate(vertex(border + M_POCKET_MOUTH * slice,border),vertex(border,border - recoil));
	t->bounds[1] = lineCreate(vertex(border,border - recoil),vertex(border - recoil,border));
	t->bounds[2] = lineCreate(vertex(border - recoil,border),vertex(border,border + M_POCKET_MOUTH * slice));

	t->bounds[3] = lineCreate(vertex(border,border + M_POCKET_MOUTH * slice),vertex(border,t->height + border - M_POCKET_MOUTH * slice));

	t->bounds[4] = lineCreate(vertex(border,t->height + border - M_POCKET_MOUTH * slice),vertex(border - recoil,t->height + border + slice - recoil));
	t->bounds[5] = lineCreate(vertex(border - recoil,t->height + border + slice - recoil),vertex(border + slice - recoil,t->height + border + recoil));
	t->bounds[6] = lineCreate(vertex(border + slice - recoil,t->height + border + recoil),vertex(border + M_POCKET_MOUTH * slice,t->height + border));

	t->bounds[7] = lineCreate(vertex(border + M_POCKET_MOUTH * slice,t->height + border),vertex(t->width / 2.0 + border - MSQRT_2 * 0.5 * M_POCKET_MOUTH * slice,t->height + border));

	t->bounds[8] = lineCreate(vertex(t->width / 2.0 + border - MSQRT_2 * 0.5 * M_POCKET_MOUTH * slice,t->height + border),vertex(t->width / 2.0 + border - MSQRT_2 * 0.5 * slice,t->height + border + recoil));
	t->bounds[9] = lineCreate(vertex(t->width / 2.0 + border - MSQRT_2 * 0.5 * slice,t->height + border + recoil),vertex(t->width / 2.0 + border + MSQRT_2 * 0.5 * slice,t->height + border + recoil));
	t->bounds[10] = lineCreate(vertex(t->width / 2.0 + border + MSQRT_2 * 0.5 * slice,t->height + border + recoil),vertex(t->width / 2.0 + border + MSQRT_2 * 0.5 * M_POCKET_MOUTH * slice,t->height + border));

	t->bounds[11] = lineCreate(vertex(t->width / 2.0 + border + MSQRT_2 * 0.5 * M_POCKET_MOUTH * slice,t->height + border),vertex(t->width + border - M_POCKET_MOUTH * slice,t->height + border));

	t->bounds[12] = lineCreate(vertex(t->width + border - M_POCKET_MOUTH * slice,t->height + border), vertex(t->width + border - slice + recoil,t->height + border + recoil));
	t->bounds[13] = lineCreate(vertex(t->width + border - slice + recoil,t->height + border + recoil), vertex(t->width + border + recoil,t->height + border + slice - recoil));
	t->bounds[14] = lineCreate(vertex(t->width + border + recoil,t->height + border + slice - recoil), vertex(t->width + border,t->height + border - M_POCKET_MOUTH * slice));

	t->bounds[15] = lineCreate(vertex(t->width + border,t->height + border - M_POCKET_MOUTH * slice), vertex(t->width + border,border + M_POCKET_MOUTH * slice));

	t->bounds[16] = lineCreate(vertex(t->width + border,border + M_POCKET_MOUTH * slice),vertex(t->width + border + recoil,border + slice - recoil));
	t->bounds[17] = lineCreate(vertex(t->width + border + recoil,border + slice - recoil),vertex(t->width + border - slice + recoil,border - recoil));
	t->bounds[18] = lineCreate(vertex(t->width + border - slice + recoil,border - recoil),vertex(t->width + border - M_POCKET_MOUTH * slice,border));

	t->bounds[19] = lineCreate(vertex(t->width + border - M_POCKET_MOUTH * slice,border),vertex(t->width / 2.0 + border + MSQRT_2 * 0.5 * M_POCKET_MOUTH * slice,border));

	t->bounds[20] = lineCreate(vertex(t->width / 2.0 + border + MSQRT_2 * 0.5 * M_POCKET_MOUTH * slice,border),vertex(t->width / 2.0 + border + MSQRT_2 * 0.5 * slice,border - recoil));
	t->bounds[21] = lineCreate(vertex(t->width / 2.0 + border + MSQRT_2 * 0.5 * slice,border - recoil),vertex(t->width / 2.0 + border - MSQRT_2 * 0.5 * slice,border - recoil));
	t->bounds[22] = lineCreate(vertex(t->width / 2.0 + border - MSQRT_2 * 0.5 * slice,border - recoil),vertex(t->width / 2.0 + border - MSQRT_2 * 0.5 * M_POCKET_MOUTH * slice,border));

	t->bounds[23] = lineCreate(vertex(t->width / 2.0 + border - MSQRT_2 * 0.5 * M_POCKET_MOUTH * slice,border),vertex(border + M_POCKET_MOUTH * slice,border));

	#ifdef DEBUG
		border = tableGetBorder(t);
		glBegin(GL_LINES);
		glColor3f(1,0,0);
			int	i = 0;
			for ( i = 0; i < TABLE_BOUNDS; i++ ) {
				glVertex2f(t->bounds[i]->p1.x + border,t->bounds[i]->p1.y + border);
				glVertex2f(t->bounds[i]->p2.x + border,t->bounds[i]->p2.y + border);
			}
		glEnd();
	#endif
}

void tablePaint(PTable t) {
	TFloat	border = tableGetBorder(t);
	TFloat	slice = t->pocketSize / MSQRT_2;

	glLoadIdentity();

	glColor3f(0.34,0.18,0.13);
	glBegin(GL_QUADS);
		glVertex2f(0,0);
		glVertex2f(0,t->totalHeight);
		glVertex2f(t->totalWidth,t->totalHeight);
		glVertex2f(t->totalWidth,0);
	glEnd();

	glColor3f(0.0,0.4,0.0);
	glBegin(GL_QUADS);
		glVertex2f(border,border);
		glVertex2f(border,t->height + border);
		glVertex2f(t->width + border,t->height + border);
		glVertex2f(t->width + border,border);

		glVertex2f(border + t->bounds[0]->p1.x,border + t->bounds[0]->p1.y);
		glVertex2f(border + t->bounds[1]->p1.x,border + t->bounds[1]->p1.y);
		glVertex2f(border + t->bounds[2]->p1.x,border + t->bounds[2]->p1.y);
		glVertex2f(border + t->bounds[3]->p1.x,border + t->bounds[3]->p1.y);

		glVertex2f(border + t->bounds[4]->p1.x,border + t->bounds[4]->p1.y);
		glVertex2f(border + t->bounds[5]->p1.x,border + t->bounds[5]->p1.y);
		glVertex2f(border + t->bounds[6]->p1.x,border + t->bounds[6]->p1.y);
		glVertex2f(border + t->bounds[7]->p1.x,border + t->bounds[7]->p1.y);

		glVertex2f(border + t->bounds[8]->p1.x,border + t->bounds[8]->p1.y);
		glVertex2f(border + t->bounds[9]->p1.x,border + t->bounds[9]->p1.y);
		glVertex2f(border + t->bounds[10]->p1.x,border + t->bounds[10]->p1.y);
		glVertex2f(border + t->bounds[11]->p1.x,border + t->bounds[11]->p1.y);

		glVertex2f(border + t->bounds[12]->p1.x,border + t->bounds[12]->p1.y);
		glVertex2f(border + t->bounds[13]->p1.x,border + t->bounds[13]->p1.y);
		glVertex2f(border + t->bounds[14]->p1.x,border + t->bounds[14]->p1.y);
		glVertex2f(border + t->bounds[15]->p1.x,border + t->bounds[15]->p1.y);

		glVertex2f(border + t->bounds[16]->p1.x,border + t->bounds[16]->p1.y);
		glVertex2f(border + t->bounds[17]->p1.x,border + t->bounds[17]->p1.y);
		glVertex2f(border + t->bounds[18]->p1.x,border + t->bounds[18]->p1.y);
		glVertex2f(border + t->bounds[19]->p1.x,border + t->bounds[19]->p1.y);

		glVertex2f(border + t->bounds[20]->p1.x,border + t->bounds[20]->p1.y);
		glVertex2f(border + t->bounds[21]->p1.x,border + t->bounds[21]->p1.y);
		glVertex2f(border + t->bounds[22]->p1.x,border + t->bounds[22]->p1.y);
		glVertex2f(border + t->bounds[23]->p1.x,border + t->bounds[23]->p1.y);
	glEnd();

	glColor3f(0,0,0);
	slice /= 2.0;
	drawCircle(t->pocketSize / 2.0,border - MCOS_PI_4 * slice,border - MSIN_PI_4 * slice);
	drawCircle(t->pocketSize / 2.0,border - MCOS_PI_4 * slice,t->height + border + MSIN_PI_4 * slice);
	drawCircle(t->pocketSize / 2.0,t->width + border + MCOS_PI_4 * slice,t->height + border + MSIN_PI_4 * slice);
	drawCircle(t->pocketSize / 2.0,t->width + border + MCOS_PI_4 * slice,border - MSIN_PI_4 * slice);
	drawCircle(t->pocketSize / 2.0,t->totalWidth / 2.0,border - t->pocketSize / 2.0);
	drawCircle(t->pocketSize / 2.0,t->totalWidth / 2.0,t->height + border + t->pocketSize / 2.0);

	#ifdef	DEBUG
		border = tableGetBorder(t);
		glLoadIdentity();
		glBegin(GL_LINES);
			glColor3f(1,0,0);
			int	i = 0;
			for ( i = 0; i < TABLE_BOUNDS; i++ ) {
				glVertex2f(t->bounds[i]->p1.x + border,t->bounds[i]->p1.y + border);
				glVertex2f(t->bounds[i]->p2.x + border,t->bounds[i]->p2.y + border);
			}
		glEnd();
	#endif
}

int tableCollided(PTable table, PBall ball) {
	unsigned	hit = 0, i;
	for ( i = 0; i < TABLE_BOUNDS; i++ ) {
		if ( lineBallCollided(table->bounds[i],ball) ) {
			hit |= (unsigned)1 << i;
		}
	}
	return hit;
}

int tableInPocket(PTable table, PBall ball, int hit) {
	return hit & table->pockets;
}

TFloat tableCollisionTime(PTable table, PBall ball, int hit) {
	TFloat	time = 0.0;
	int		i;
	for ( i = 0; i < TABLE_BOUNDS; i++ ) {
		if ( hit & 1 ) {
			TFloat	t = 0.0;
			if ( table->bounds[i]->p1.x == table->bounds[i]->p2.x ) {
				// vertical.
				TFloat	d = ball->radius - distancePointToLineSegment(vertex(ball->x,ball->y),table->bounds[i]);
				t = fabs(d / ballSpeedX(ball));
			} else if ( table->bounds[i]->p1.y == table->bounds[i]->p2.y ) {
				// horizontal.
				TFloat	d = ball->radius - distancePointToLineSegment(vertex(ball->x,ball->y),table->bounds[i]);
				t = fabs(d / ballSpeedY(ball));
			} else {
				// rotate.
				TFloat	d, a = -atan(angularCoef(table->bounds[i]->p1,table->bounds[i]->p2));
				TFloat	mr[2][2];
				TLine	l;
				TVertex	c;
				mr[0][0] = cos(a);
				mr[0][1] = -sin(a);
				mr[1][0] = -mr[0][1];
				mr[1][1] = mr[0][0];
				c.x = mr[0][0] * ball->x + mr[0][1] * ball->y;
				c.y = mr[1][0] * ball->x + mr[1][1] * ball->y;
				l.p1.x = mr[0][0] * table->bounds[i]->p1.x + mr[0][1] * table->bounds[i]->p1.y;
				l.p1.y = mr[1][0] * table->bounds[i]->p1.x + mr[1][1] * table->bounds[i]->p1.y;
				l.p2.x = mr[0][0] * table->bounds[i]->p2.x + mr[0][1] * table->bounds[i]->p2.y;
				l.p2.y = mr[1][0] * table->bounds[i]->p2.x + mr[1][1] * table->bounds[i]->p2.y;
				// distance.
				d = ball->radius - distancePointToLineSegment(c,&l);
				// time.
				t = fabs(d / (sin(ball->angle - a) * ballSpeed(ball)));
			}
			if ( t > time ) {
				time = t;
			}
		}
		hit = hit >> 1;
	}
	return time;
}

int tableCollision(PTable table, PBall ball, int hit) {
	if ( !hit ) {
		return 0;
	}
	int	bound = 0, bound2, i = 0, count = 0;
	for ( i = 0; i < TABLE_BOUNDS; i++ ) {
		if ( hit & 1 ) {
			if ( count >= 1 ) {
				bound2 = i;
			} else {
				bound = i;
			}
			count++;
		}
		hit = hit >> 1;
	}
	TFloat	ra;
	if ( count == 1 ) {
		if ( table->bounds[bound]->p1.x == table->bounds[bound]->p2.x ) {
			// vertical.
			if ( ball->x >= table->bounds[bound]->p1.x ) {
				if ( ball->angle <= MPI ) {
					ra = -M_PI_2;
				} else {
					ra = - M3_PI_2;
				}
			} else {
				if ( ball->angle <= MPI ) {
					ra = - MPI_2;
				} else {
					ra = - M3_PI_2;
				}
			}
			ball->angle += ra;
			ball->angle *= -1.0;
			ball->angle -= ra;
		} else if ( table->bounds[bound]->p1.y == table->bounds[bound]->p2.y ) {
			// horizontal.
			if ( ball->y >= table->bounds[bound]->p1.y ) {
				if ( ball->angle < M3_PI_2 ) {
					ra = - M2_PI;
				} else {
					ra = - MPI;
				}
			} else {
				if ( ball->angle < MPI_2 ) {
					ra = 0.0;
				} else {
					ra = - MPI;
				}
			}
			ball->angle += ra;
			ball->angle *= -1.0;
			ball->angle -= ra;
		} else {
			// inclined.
			TFloat	sa = -atan(angularCoef(table->bounds[bound]->p1,table->bounds[bound]->p2));
			TFloat	mr[2][2];
			TLine	l;
			TVertex	c;
			mr[0][0] = cos(sa);
			mr[0][1] = -sin(sa);
			mr[1][0] = -mr[0][1];
			mr[1][1] = mr[0][0];
//			c.x = mr[0][0] * ball->x + mr[0][1] * ball->y;
			c.y = mr[1][0] * ball->x + mr[1][1] * ball->y;
			l.p1.x = mr[0][0] * table->bounds[bound]->p1.x + mr[0][1] * table->bounds[bound]->p1.y;
			l.p1.y = mr[1][0] * table->bounds[bound]->p1.x + mr[1][1] * table->bounds[bound]->p1.y;
			l.p2.x = mr[0][0] * table->bounds[bound]->p2.x + mr[0][1] * table->bounds[bound]->p2.y;
			l.p2.y = mr[1][0] * table->bounds[bound]->p2.x + mr[1][1] * table->bounds[bound]->p2.y;
			// apply the rotation.
			ball->angle -= sa;
			if ( c.y >= l.p1.y ) {
				if ( ball->angle < M3_PI_2 ) {
					ra = - M3_PI_2;
				} else {
					ra = - M2_PI;
				}
			} else {
				if ( ball->angle < MPI_2 ) {
					ra = 0.0;
				} else {
					ra = -MPI;
				}
			}
			ball->angle -= ra;
			ball->angle *= -1.0;
			ball->angle += ra - sa;
		}
	} else {
		// corner.
		TVertex	corner;
		if ( sameVertex(table->bounds[bound]->p1,table->bounds[bound2]->p1) ) {
			corner = table->bounds[bound]->p1;
		} else if ( sameVertex(table->bounds[bound]->p1,table->bounds[bound2]->p2) ) {
			corner = table->bounds[bound]->p1;
		} else {
			corner = table->bounds[bound]->p2;
		}
		TFloat	sa = atan(-1.0 / angularCoef(corner,vertex(ball->x,ball->y)));
		TFloat	mr[2][2];
		TLine	l;
		TVertex	c;
		mr[0][0] = cos(sa);
		mr[0][1] = -sin(sa);
		mr[1][0] = -mr[0][1];
		mr[1][1] = mr[0][0];
		c.x = mr[0][0] * ball->x + mr[0][1] * ball->y;
		c.y = mr[1][0] * ball->x + mr[1][1] * ball->y;
		l.p1.x = mr[0][0] * corner.x + mr[0][1] * corner.y;
		l.p1.y = mr[1][0] * corner.x + mr[1][1] * corner.y;
		l.p2.x = mr[0][0] * corner.x + mr[0][1] * corner.y;
		l.p2.y = mr[1][0] * corner.x + mr[1][1] * corner.y;
		ball->angle -= sa;
		if ( c.y >= l.p1.y ) {
			if ( ball->angle < M3_PI_2 ) {
				ra = - M3_PI_2;
			} else {
				ra = - M2_PI;
			}
		} else {
			if ( ball->angle < MPI_2 ) {
				ra = 0.0;
			} else {
				ra = - MPI;
			}
		}
		ball->angle -= ra;
		ball->angle *= -1.0;
		ball->angle += ra - sa;
	}
	ball->angle /= M2_PI;
	ball->angle = (ball->angle - trunc(ball->angle)) * M2_PI;
	ball->speed *= M_TABLE_HIT_SPEED_RATIO;
	return hit;
}

TFloat tableGetBorder(PTable table) {
	return (table->totalWidth - table->width) / 2.0;
}

TFloat tableGetAtrCoef(PTable table) {
	return table->atrCoef;
}

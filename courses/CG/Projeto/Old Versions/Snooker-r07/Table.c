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

	TFloat	slice = pocketSize / MSQRT_2;
	TFloat	recoil = slice;
//	TFloat	border = tableGetBorder(t);
	TFloat	border = 0.0;

	t->bounds[0] = lineCreate(vertex(border - slice,border),vertex(border + slice - recoil,recoil));
	t->bounds[1] = lineCreate(vertex(border + slice - recoil,border - recoil),vertex(recoil,slice - recoil));
	t->bounds[2] = lineCreate(vertex(border + recoil,border + slice - recoil),vertex(border,border + slice));

	t->bounds[3] = lineCreate(vertex(border,border + slice),vertex(border,t->height + border - slice));

	t->bounds[4] = lineCreate(vertex(border,t->height + border - slice),vertex(border - recoil,t->height + border + slice - recoil));
	t->bounds[5] = lineCreate(vertex(border - recoil,t->height + border + slice - recoil),vertex(border + slice - recoil,t->height + border + recoil));
	t->bounds[6] = lineCreate(vertex(border + slice - recoil,t->height + border + recoil),vertex(border + slice,t->height + border));

	t->bounds[7] = lineCreate(vertex(border + slice,t->height + border),vertex(t->totalWidth / 2.0 - MSQRT_2 * 0.7 * slice,t->height + border));

	t->bounds[8] = lineCreate(vertex(t->totalWidth / 2.0 - MSQRT_2 * 0.7 * slice,t->height + border),vertex(t->totalWidth / 2.0 - MSQRT_2 * 0.5 * slice,t->height + border + recoil));
	t->bounds[9] = lineCreate(vertex(t->totalWidth / 2.0 - MSQRT_2 * 0.5 * slice,t->height + border + recoil),vertex(t->totalWidth / 2.0 + MSQRT_2 * 0.5 * slice,t->height + border + recoil));
	t->bounds[10] = lineCreate(vertex(t->totalWidth / 2.0 + MSQRT_2 * 0.5 * slice,t->height + border + recoil),vertex(t->totalWidth / 2.0 + MSQRT_2 * 0.7 * slice,t->height + border));

	t->bounds[11] = lineCreate(vertex(t->totalWidth / 2.0 + MSQRT_2 * 0.7 * slice,t->height + border),vertex(t->width + border - slice,t->height + border));

	t->bounds[12] = lineCreate(vertex(t->width + border - slice,t->height + border), vertex(t->width + border - slice + recoil,t->height + border + recoil));
	t->bounds[13] = lineCreate(vertex(t->width + border - slice + recoil,t->height + border + recoil), vertex(t->width + border + recoil,t->height + border + slice - recoil));
	t->bounds[14] = lineCreate(vertex(t->width + border + recoil,t->height + border + slice - recoil), vertex(t->width + border,t->height + border - slice));

	t->bounds[15] = lineCreate(vertex(t->width + border,t->height + border - slice), vertex(t->width + border,border + slice));

	t->bounds[16] = lineCreate(vertex(t->width + border,border + slice),vertex(t->width + border + recoil,border + slice - recoil));
	t->bounds[17] = lineCreate(vertex(t->width + border + recoil,border + slice - recoil),vertex(t->width + border - slice + recoil,border - recoil));
	t->bounds[18] = lineCreate(vertex(t->width + border - slice + recoil,border - recoil),vertex(t->width + border - slice,border));

	t->bounds[19] = lineCreate(vertex(t->width + border - slice,border),vertex(t->width / 2.0 + border + MSQRT_2 * 0.7 * slice	,border));

	t->bounds[20] = lineCreate(vertex(t->width / 2.0 + border + MSQRT_2 * 0.7 * slice	,border),vertex(t->width / 2.0 + border + MSQRT_2 * 0.5 * slice,border - recoil));
	t->bounds[21] = lineCreate(vertex(t->width / 2.0 + border + MSQRT_2 * 0.5 * slice	,border - recoil),vertex(t->width / 2.0 + border - MSQRT_2 * 0.5 * slice,border - recoil));
	t->bounds[22] = lineCreate(vertex(t->width / 2.0 + border - MSQRT_2 * 0.5 * slice	,border - recoil),vertex(t->width / 2.0 + border - MSQRT_2 * 0.7 * slice,border));

	t->bounds[23] = lineCreate(vertex(t->width / 2.0 + border - MSQRT_2 * 0.7 * slice	,border),vertex(border + slice,border));
}

void tablePaint(PTable t) {
	TFloat	border = tableGetBorder(t);
	TFloat	slice = t->pocketSize / MSQRT_2;
	TFloat	recoil = slice;

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

		glVertex2f(t->bounds[8]->p1.x,border + t->bounds[8]->p1.y);
		glVertex2f(t->bounds[9]->p1.x,border + t->bounds[9]->p1.y);
		glVertex2f(t->bounds[10]->p1.x,border + t->bounds[10]->p1.y);
		glVertex2f(t->bounds[11]->p1.x,border + t->bounds[11]->p1.y);

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

TFloat tableCollisionTime(PTable table, PBall ball, int hit) {
	TFloat	time = 0.0;
	int		i;
	for ( i = 0; i < TABLE_BOUNDS; i++ ) {
		if ( hit & 1 ) {
			TFloat	d = distancePointToLine(vertex(ball->x,ball->y),table->bounds[i]);
			TFloat	t = fabs((ball->radius - d) / ball->speed);
			if ( t > time ) {
				time = t;
			}
		}
		hit = hit >> 1;
	}
	return time;
}

int tableCollision(PTable table, PBall ball, int hit) {
	if ( hit & M_HIT_LEFT ) {
		if ( ball->angle <= MPI ) {
			ball->angle -= MPI_2;
			ball->angle *= -1;
			ball->angle += MPI_2;
		} else {
			ball->angle -= M3_PI_2;
			ball->angle *= -1;
			ball->angle += M3_PI_2;
		}
	}
	if ( hit & M_HIT_RIGHT ) {
		if ( ball->angle <= MPI ) {
			ball->angle -= MPI_2;
			ball->angle *= -1;
			ball->angle += MPI_2;
		} else {
			ball->angle -= M3_PI_2;
			ball->angle *= -1;
			ball->angle += M3_PI_2;
		}
	}
	if ( hit & M_HIT_TOP ) {
		ball->angle = M2_PI - ball->angle;
	}
	if ( hit & M_HIT_BOTTOM ) {
		ball->angle = M2_PI - ball->angle;
	}
	ball->speed *= M_TABLE_HIT_SPEED_RATIO;
	return hit;
}

TFloat tableGetBorder(PTable table) {
	return (table->totalWidth - table->width) / 2.0;
}

TFloat tableGetAtrCoef(PTable table) {
	return table->atrCoef;
}

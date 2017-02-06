#include <stdlib.h>
#include <math.h>
#include "Ball.h"
#include <OpenGL/gl.h>

PBall ballCreate(char* name) {
	PBall ball = malloc(sizeof(TBall));
	ball->name = name;
	ball->angle = ball->speed = 0;
	ball->flags = 0;
	return ball;
}

void ballDestroy(PBall *ball) {
	if ( ball && *ball ) {
		free((*ball)->name);
		free(*ball);
		*ball = 0;
	}
}

void ballSetFlag(PBall ball, unsigned flag) {
	ball->flags |= flag;
	if ( flag == BALL_DEAD ) {
		ball->speed = 0.0;
		ball->angle = 0.0;
	}
}

void ballClearFlag(PBall ball, unsigned flag) {
	ball->flags &= ~flag;
}

int ballGetFlag(PBall ball, unsigned flag) {
	return ball->flags & flag;
}

void ballSetXY(PBall ball, TFloat x, TFloat y) {
	ball->x = x;
	ball->y = y;
}

void ballSetup(PBall ball, TFloat radius, TFloat mass) {
	ball->radius = radius;
	ball->mass = mass;
}

void ballPaint(PBall ball, TFloat border) {
	glColor3f(ball->r,ball->g,ball->b);
	drawCircle(ball->radius,ball->x + border,ball->y + border);
	if ( ballGetFlag(ball,BALL_DRAW_ERROR_SIGNAL) ) {
		glColor3f(1,0,0);
		glBegin(GL_LINE_LOOP);
			glVertex2f(border + ball->x - ball->radius,border + ball->y - ball->radius);
			glVertex2f(border + ball->x + ball->radius,border + ball->y - ball->radius);
			glVertex2f(border + ball->x + ball->radius,border + ball->y + ball->radius);
			glVertex2f(border + ball->x - ball->radius,border + ball->y + ball->radius);
		glEnd();
	}
	if ( ballGetFlag(ball,BALL_DRAW_SPEED_VECTOR) ) {
		glColor3f(0,1,0);
		glBegin(GL_LINES);
			glVertex2f(border + ball->x,border + ball->y);
			glVertex2f(border + ball->x + ballSpeedX(ball),border + ball->y + ballSpeedY(ball));
		glEnd();
	}
}

TFloat ballDistance(PBall b1, PBall b2) {
	return sqrt(sqr(b2->x - b1->x) + sqr(b2->y - b1->y));
}

int ballCollided(PBall b1, PBall b2) {
	return ballDistance(b1,b2) <= b2->radius + b1->radius;
}

TFloat ballCollisionRelativeAngle(PBall b1, PBall b2) {
	return atan((b2->y - b1->y) / (b2->x - b1->x));
}

TFloat ballCollisionTime(PBall b1, PBall b2) {
	// desired distance between the balls.
	TFloat	s = b1->radius + b2->radius;
	// speed delta.
	TFloat	dvx = ballSpeedX(b1) - ballSpeedX(b2), dvy = ballSpeedY(b1) - ballSpeedY(b2);
	// axes delta.
	TFloat	dx = b1->x - b2->x, dy = b1->y - b2->y;
	// equation coeficients.
	TFloat	a = sqr(dvx) + sqr(dvy);
	TFloat	b = 2 * ((dvx * dx) + (dvy * dy));
	TFloat	c = sqr(dx) + sqr(dy) - sqr(s);
	TFloat	d = sqr(b) - 4 * a * c;
	// time.
	TFloat	t = (-b - sqrt(d)) / (2 * a);
	return	fabs(t);
}

int ballCollision(PBall b1, PBall b2) {
	TFloat	dx = b1->x - b2->x, dy = b1->y - b2->y;
	TFloat	d = sqrt(sqr(dx) + sqr(dy));
	TFloat	vp1, vp2, vx1, vx2, vy1, vy2;
	vx1 = ballSpeedX(b1);
	vx2 = ballSpeedX(b2);
	vy1 = ballSpeedY(b1);
	vy2 = ballSpeedY(b2);
	vp1 = vx1 * dx / d + vy1 * dy / d;
	vp2 = vx2 * dx / d + vy2 * dy / d;

	TFloat	distance = sqrt(sqr(dx) + sqr(dy));

//	Unit vector in the direction of the collision.
	TFloat	ax = dx / distance, ay = dy / distance;

//	Projection of the velocities in these axes.
	TFloat	va1 = vx1 * ax + vy1 * ay, vb1 = -vx1 * ay + vy1 * ax;
	TFloat	va2 = vx2 * ax + vy2 * ay, vb2 = -vx2 * ay + vy2 * ax;

//	New velocities in these axes (after collision): ed<=1,  for elastic collision ed=1.
	TFloat	vaP1 = va1 + (1.0 + M_BALL_ELASTIC_COEFFICIENT) * (va2 - va1) / (1.0 + b1->mass / b2->mass);
	TFloat	vaP2 = va2 + (1.0 + M_BALL_ELASTIC_COEFFICIENT) * (va1 - va2) / (1.0 + b2->mass / b1->mass);

//	Undo the projections.
	vx1 = vaP1 * ax - vb1 * ay; vy1 = vaP1 * ay + vb1 * ax;	// new vx,vy for ball 1 after collision.
	vx2 = vaP2 * ax - vb2 * ay; vy2 = vaP2 * ay + vb2 * ax;	// new vx,vy for ball 2 after collision.

	b1->speed = sqrt(sqr(vx1) + sqr(vy1));
	b1->angle = acos(fabs(vx1 / b1->speed));

	b2->speed = sqrt(sqr(vx2) + sqr(vy2));
	b2->angle = acos(fabs(vx2 / b2->speed));

	if ( vx1 < 0 && vy1 < 0 )
		b1->angle = MPI + b1->angle;
	else if ( vx1 < 0 )
		b1->angle = MPI - b1->angle;
	else if ( vy1 < 0 )
		b1->angle = M2_PI - b1->angle;

	if ( vx2 < 0 && vy2 < 0 )
		b2->angle = MPI + b2->angle;
	else if ( vx2 < 0 )
		b2->angle = MPI - b2->angle;
	else if ( vy2 < 0 )
		b2->angle = M2_PI - b2->angle;

	if ( b1->angle < 0 )
		b1->angle = M2_PI + b1->angle;
	if ( b2->angle < 0 )
		b2->angle = M2_PI + b2->angle;

	return 1;
}

void ballApplyMovement(PBall ball, TFloat angle, TFloat speed) {
	ball->angle = angle;
	ball->speed = speed;
}

TFloat ballSpeedX(PBall ball) {
	return cos(ball->angle) * ball->speed;
}

TFloat ballSpeedY(PBall ball) {
	return sin(ball->angle) * ball->speed;
}

TFloat ballSpeedAfter(PBall ball, TFloat m, TFloat time) {
	TFloat	speed = ball->speed - m * M_G * time;
	return ( speed < 0 ) ? 0.0 : speed;
}

TFloat ballSpeedXAfter(PBall ball, TFloat m, TFloat time) {
	return cos(ball->angle) * ballSpeedAfter(ball,m,time);
}

TFloat ballSpeedYAfter(PBall ball, TFloat m, TFloat time) {
	return sin(ball->angle) * ballSpeedAfter(ball,m,time);
}

int ballStep(PBall ball, TFloat m, TFloat time) {
	if ( ball->speed ) {
		TFloat	sx, sy;
		sx = ballSpeedXAfter(ball,m,time);
		sy = ballSpeedYAfter(ball,m,time);
		ball->x += (sx + ballSpeedX(ball)) * time / 2.0;
		ball->y += (sy + ballSpeedY(ball)) * time / 2.0;
		ball->speed = ballSpeedAfter(ball,m,time);
	}
	#ifdef DEBUG
		printf("X=%.6f Y=%.6f VX=%.6f VY=%.6f R=%.6f time=%.6f\n",ball->x,ball->y,ballSpeedX(ball),ballSpeedY(ball),ball->radius,time);
	#endif
	return ball->speed;
}

void ballSetColor(PBall ball, float r, float g, float b) {
	ball->r = r;
	ball->g = g;
	ball->b = b;
}

TFloat ballSpeed(PBall ball) {
	return ball->speed;
}

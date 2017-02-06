#include	"SnookerUtils.h"
#include	<OpenGL/gl.h>

TFloat sqr(TFloat x) {
	return x * x;
}

TVertex vertex(TFloat x, TFloat y) {
	TVertex	v;
	v.x = x;
	v.y = y;
	return v;
}

void drawCircle(TFloat radius, TFloat xc, TFloat yc) {
	glBegin(GL_TRIANGLE_FAN);
		TFloat	x, y;
		TFloat	angle;
		for ( angle = 0; angle < M2_PI; angle += 0.2 ) {
			x = xc + radius * cos(angle);
			y = yc + radius * sin(angle);
			glVertex2f(x,y);
		}
	glEnd();
}

TFloat distancePointToLine(TVertex p, PLine l) {
	TFloat	a, b, c;
	a = l->p1.y - l->p2.y;
	b = l->p2.x - l->p1.x;
	c = (l->p1.x * l->p2.y) - (l->p1.y * l->p2.x);
	return fabs((a * p.x) + (b * p.y) + c) / sqrt(sqr(a) + sqr(b));
}

#include	"SnookerUtils.h"
#include	"OpenGL/gl.h"

inline TFloat radToDegree(TFloat rad) {
	return rad * 360.0 / M2_PI;
}

inline TFloat pitagorasH(TFloat c1, TFloat c2) {
	return sqrt(sqr(c1) + sqr(c2));
}

inline TFloat pitagorasC(TFloat h, TFloat c) {
	return sqrt(sqr(h) - sqr(c));
}

inline TFloat sqr(TFloat x) {
	return x * x;
}

inline TFloat min(TFloat a, TFloat b) {
	return ( a <= b ) ? a : b;
}

inline TVertex vertex(TFloat x, TFloat y) {
	TVertex	v;
	v.x = x;
	v.y = y;
	return v;
}

inline int sameVertex(TVertex v1, TVertex v2) {
	return v1.x == v2.x && v1.y == v2.y;
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

inline double angularCoef(TVertex v1, TVertex v2) {
	return (v2.y - v1.y) / (v2.x - v1.x);
}

TFloat distancePointToPoint(TVertex p1, TVertex p2) {
	return pitagorasH(p1.x - p2.x,p1.y - p2.y);
}

TFloat distancePointToLine(TVertex p, PLine l) {
	TFloat	a, b, c;
	a = l->p1.y - l->p2.y;
	b = l->p2.x - l->p1.x;
	c = (l->p1.x * l->p2.y) - (l->p1.y * l->p2.x);
	return fabs((a * p.x) + (b * p.y) + c) / sqrt(sqr(a) + sqr(b));
}

TFloat distancePointToLineSegment(TVertex p, PLine l) {
	TVertex	i;
	if ( l->p1.x == l->p2.x ) {
		// vertical segment.
		i.x = l->p1.x;
		i.y = p.y;
		if ( (i.y > l->p1.y && i.y > l->p2.y) || (i.y < l->p1.y && i.y < l->p2.y) ) {
			// away of the line segment.
			return min(distancePointToPoint(p,l->p1),distancePointToPoint(p,l->p2));
		}
	} else if ( l->p1.y == l->p2.y ) {
		// horizontal segment.
		i.x = p.x;
		i.y = l->p1.y;
		if ( (i.x > l->p1.x && i.x > l->p2.x) || (i.x < l->p1.x && i.x < l->p2.x) ) {
			// away of the line segment.
			return min(distancePointToPoint(p,l->p1),distancePointToPoint(p,l->p2));
		}
	} else {
		TFloat	m, cr, cs;
		m = (l->p2.y - l->p1.y) / (l->p2.x - l->p1.x);
		cr = l->p1.y - (l->p1.x * m);
		cs = p.y - (p.x / m);
		i.x = (cr - cs) * sqr(m) / (-1.0 - sqr(m));
		if ( (i.x > l->p1.x && i.x > l->p2.x) || (i.x < l->p1.x && i.x < l->p2.x) ) {
			// away of the line segment.
			return min(distancePointToPoint(p,l->p1),distancePointToPoint(p,l->p2));
		} else {
			// inside the line segment.
			i.y = m * i.x + cr;
		}
	}
	// inside the line segment.
	return distancePointToPoint(p,i);
}

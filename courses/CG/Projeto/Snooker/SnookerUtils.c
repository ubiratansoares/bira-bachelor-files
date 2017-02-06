#include	"SnookerUtils.h"
#include	<OpenGL/gl.h>

inline TFloat radToDegree(TFloat rad) {
	return rad * 360.0 / D_2_PI;
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

inline TVertex3D vertex3D(TFloat x, TFloat y, TFloat z) {
	TVertex3D v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

inline TUVertex uvertex(unsigned x, unsigned y) {
	TUVertex	v;
	v.x = x;
	v.y = y;
	return v;
}

inline int sameVertex(TVertex v1, TVertex v2) {
	return v1.x == v2.x && v1.y == v2.y;
}

inline int sameUVertex(TUVertex v1, TUVertex v2) {
	return v1.x == v2.x && v1.y == v2.y;
}

void drawCircle2D(TFloat radius, TFloat xc, TFloat yc) {
	glBegin(GL_TRIANGLE_FAN);
		TFloat	x, y;
		TFloat	angle;
		for ( angle = 0; angle < D_2_PI; angle += 0.2 ) {
			x = xc + radius * cos(angle);
			y = yc + radius * sin(angle);
			glVertex2f(x,y);
		}
	glEnd();
}

void drawCircle3Dxy(TFloat radius, TFloat xc, TFloat yc, TFloat z) {
	glBegin(GL_TRIANGLE_FAN);
		TFloat	x, y;
		TFloat	angle;
		for ( angle = 0; angle < D_2_PI; angle += 0.2 ) {
			x = xc + radius * cos(angle);
			y = yc + radius * sin(angle);
			glVertex3f(x,y,z);
		}
	glEnd();
}

void drawWiredCircleSection2D(TFloat radius, TFloat xc, TFloat yc, TFloat startAngle, TFloat endAngle, TFloat slice) {
	if ( startAngle < 0 ) {
		startAngle = D_PI - startAngle;
	}
	if ( endAngle < 0 ) {
		endAngle = D_PI - endAngle;
	}
	slice = fabs(slice);
	if ( endAngle < startAngle ) {
		slice = -slice;
	}
	glBegin(GL_LINE_STRIP);
		TFloat	x, y;
		TFloat	angle;
		for ( angle = startAngle; angle < endAngle; angle += slice ) {
			x = xc + radius * cos(angle);
			y = yc + radius * sin(angle);
			glVertex2f(x,y);
		}
		glVertex2f(xc + radius * cos(endAngle),yc + radius * sin(endAngle));
	glEnd();
}

void drawWiredCircleSection3Dxy(TFloat radius, TFloat xc, TFloat yc, TFloat startAngle, TFloat endAngle, TFloat slice, TFloat z) {
	if ( startAngle < 0 ) {
		startAngle = D_PI - startAngle;
	}
	if ( endAngle < 0 ) {
		endAngle = D_PI - endAngle;
	}
	slice = fabs(slice);
	if ( endAngle < startAngle ) {
		slice = -slice;
	}
	glBegin(GL_LINE_STRIP);
		TFloat	x, y;
		TFloat	angle;
		for ( angle = startAngle; angle < endAngle; angle += slice ) {
			x = xc + radius * cos(angle);
			y = yc + radius * sin(angle);
			glVertex3f(x,y,z);
		}
		glVertex3f(xc + radius * cos(endAngle),yc + radius * sin(endAngle),z);
	glEnd();
}

void drawCircleSectionCustom3Dxy(TFloat radius, TFloat xc, TFloat yc, TFloat startAngle, TFloat endAngle, unsigned slices, TFloat z) {
	if ( startAngle < 0 ) {
		startAngle = D_PI - startAngle;
	}
	if ( endAngle < 0 ) {
		endAngle = D_PI - endAngle;
	}
	if ( slices < 3 ) {
		slices = 3;
	}
	TFloat	slice = fabs(endAngle - startAngle) / (TFloat)slices;
	TFloat	x, y;
	TFloat	angle;
	if ( startAngle < endAngle ) {
		for ( angle = startAngle; angle < endAngle; angle += slice ) {
			x = xc + radius * cos(angle);
			y = yc + radius * sin(angle);
			glVertex3f(x,y,z);
		}
	} else {
		for ( angle = startAngle; angle > endAngle; angle -= slice ) {
			x = xc + radius * cos(angle);
			y = yc + radius * sin(angle);
			glVertex3f(x,y,z);
		}
	}
	glVertex3f(xc + radius * cos(endAngle),yc + radius * sin(endAngle),z);
}

void drawTexturedCilinderSection(TFloat angle, unsigned slices, int normals) {
	if ( slices < 3 ) {
		slices = 3;
	}
	if ( angle == 0 ) {
		return;
	}
	TFloat	angleSlice = angle / (TFloat)slices;
	TFloat	startAngle = 0.0, x, y, fX;
	fX = angle * 0.5;
	glBegin(GL_QUADS);
		x = cos(startAngle);
		y = sin(startAngle);
		if ( normals ) {
			glNormal3f(-x,-y,0.0);
		}
		x /= 2.0;
		y /= 2.0;
		for ( startAngle = 0.0; startAngle < angle - angleSlice; startAngle += angleSlice ) {
			glTexCoord2f(fX * startAngle,1.0);
			glVertex3f(x,y,0.5);
			glTexCoord2f(fX * startAngle,0.0);
			glVertex3f(x,y,-0.5);
			startAngle += angleSlice;
			x = cos(startAngle);
			y = sin(startAngle);
			if ( normals ) {
				glNormal3f(-x,-y,0.0);
			}
			x /= 2.0;
			y /= 2.0;
			glTexCoord2f(fX * startAngle,0.0);
			glVertex3f(x,y,-0.5);
			glTexCoord2f(fX * startAngle,1.0);
			glVertex3f(x,y,0.5);
		}
		glTexCoord2f(fX * startAngle,1.0);
		glVertex3f(x,y,0.5);
		glTexCoord2f(fX * startAngle,0.0);
		glVertex3f(x,y,-0.5);
		x = cos(startAngle);
		y = sin(startAngle);
		if ( normals ) {
			glNormal3f(-x,-y,0.0);
		}
		x /= 2.0;
		y /= 2.0;
		glTexCoord2f(fX * startAngle,0.0);
		glVertex3f(x,y,-0.5);
		glTexCoord2f(fX * startAngle,1.0);
		glVertex3f(x,y,0.5);
	glEnd();
}

inline double angularCoef(TVertex v1, TVertex v2) {
	return (v2.y - v1.y) / (v2.x - v1.x);
}

TFloat distancePointToPoint(TVertex p1, TVertex p2) {
	return pitagorasH(p1.x - p2.x,p1.y - p2.y);
}

TFloat distancePointToPoint3D(TVertex3D p1, TVertex3D p2) {
	return sqrt(sqr(p1.x - p2.x) + sqr(p1.y - p2.y) + sqr(p1.z - p2.z));
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

TVertex projectionPointOverLine(TVertex p, PLine l) {
	TVertex	i;
	if ( l->p1.x == l->p2.x ) {
		// vertical line.
		i.x = l->p1.x;
		i.y = p.y;
	} else if ( l->p1.y == l->p2.y ) {
		// horizontal line.
		i.x = p.x;
		i.y = l->p1.y;
	} else {
		TFloat	m, cr, cs;
		m = (l->p2.y - l->p1.y) / (l->p2.x - l->p1.x);
		cr = l->p1.y - (l->p1.x * m);
		cs = p.y - (p.x / m);
		i.x = (cr - cs) * sqr(m) / (-1.0 - sqr(m));
		i.y = m * i.x + cr;
	}
	return i;
}

inline TVector2D angleToVector(TFloat angle) {
	TVector2D	v;
	v.i = cos(angle);
	v.j = sin(angle);
	return v;
}

inline TFloat vectorToAngle(PVector2D vector) {
	TFloat	angle;
	angle = atan(vector->j / vector->i);
	if ( vector->i < 0.0 ) {
		angle += D_PI;
	}
	if ( angle < 0.0 ) {
		angle += D_2_PI;
	}
	return angle;
}

TVector2D vectorNormal(PVector2D vector) {
	TVector2D	v;
	v.i = vector->j;
	v.j = -vector->i;
	return v;
}

inline TFloat vectorNorma(PVector2D v) {
	return sqrt(sqr(v->i) + sqr(v->j));
}

TFloat vectorMultEscalar(PVector2D a, PVector2D b) {
	return a->i * b->i + a->j * b->j;
}

inline TVector2D vectorEscalarMult(PVector2D v, TFloat n) {
	TVector2D	r;
	r.i = v->i * n;
	r.j = v->j * n;
	return r;
}

inline TVector2D vectorSum(PVector2D a, PVector2D b) {
	TVector2D	v;
	v.i = a->i + b->i;
	v.j = a->j + b->j;
	return v;
}

inline TVector2D vectorDiff(PVector2D a, PVector2D b) {
	TVector2D	v;
	v.i = a->i - b->i;
	v.j = a->j - b->j;
	return v;
}

TVector2D vectorReflect(PVector2D L, PVector2D N) {
	// R = 2(N.L)N - L.
	TVector2D	R;
	R = vectorEscalarMult(N,2.0 * vectorMultEscalar(L,N));
	R = vectorDiff(&R,L);
	return R;
}

TVector2D vectorNegative(PVector2D v) {
	TVector2D	r;
	r.i = -v->i;
	r.j = -v->j;
	return r;
}

inline TFloat vectorAngleBetween(PVector2D a, PVector2D b) {
	TFloat	T;
	T = vectorMultEscalar(a,b) / (vectorNorma(a) * vectorNorma(b));
	return acos(T);
}

inline void vectorNormalize(PVector2D v) {
	TFloat	M = vectorNorma(v);
	if ( M != 1.0 ) {
		v->i /= M;
		v->j /= M;
	}
}

inline int equationSolve2(TFloat a, TFloat b, TFloat c, PFloat x1, PFloat x2) {
	TFloat	d = sqr(b) - 4 * a * c;
	if ( d < 0 ) {
		*x1 = d;
		return 0;
	}
	d = sqrt(d);
	a = a * 2;
	*x1 = (-b + d) / a;
	*x2 = (-b - d) / a;
	return 1;
}

inline GLfloat* p4ToGLfv(TFloat r, TFloat g, TFloat b, TFloat a, GLfloat *v) {
	v[0] = r;
	v[1] = g;
	v[2] = b;
	v[3] = a;
	return v;
}

void drawTexturedCube(TFloat size) {
	size /= 2.0;
	glBegin(GL_QUADS);
		glNormal3f(0.0,-1.0,0.0);
		glTexCoord2f(0.0,0.0);		glVertex3f(-size,	-size,	-size);
		glTexCoord2f(0.0,1.0);		glVertex3f(-size,	-size,	size);
		glTexCoord2f(1.0,1.0);		glVertex3f(size,	-size,	size);
		glTexCoord2f(1.0,0.0);		glVertex3f(size,	-size,	-size);

		glNormal3f(1.0,0.0,0.0);
		glTexCoord2f(0.0,0.0);		glVertex3f(size,	-size,	-size);
		glTexCoord2f(0.0,1.0);		glVertex3f(size,	-size,	size);
		glTexCoord2f(1.0,1.0);		glVertex3f(size,	size,	size);
		glTexCoord2f(1.0,0.0);		glVertex3f(size,	size,	-size);

		glNormal3f(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);		glVertex3f(-size,	size,	-size);
		glTexCoord2f(0.0,1.0);		glVertex3f(-size,	size,	size);
		glTexCoord2f(1.0,1.0);		glVertex3f(size,	size,	size);
		glTexCoord2f(1.0,0.0);		glVertex3f(size,	size,	-size);

		glNormal3f(-1.0,0.0,0.0);
		glTexCoord2f(0.0,0.0);		glVertex3f(-size,	-size,	-size);
		glTexCoord2f(0.0,1.0);		glVertex3f(-size,	-size,	size);
		glTexCoord2f(1.0,1.0);		glVertex3f(-size,	size,	size);
		glTexCoord2f(1.0,0.0);		glVertex3f(-size,	size,	-size);

		glNormal3f(0.0,0.0,1.0);
		glTexCoord2f(0.0,0.0);		glVertex3f(-size,	-size,	size);
		glTexCoord2f(0.0,1.0);		glVertex3f(size,	-size,	size);
		glTexCoord2f(1.0,1.0);		glVertex3f(size,	size,	size);
		glTexCoord2f(1.0,0.0);		glVertex3f(-size,	size,	size);

		glNormal3f(0.0,0.0,-1.0);
		glTexCoord2f(0.0,0.0);		glVertex3f(-size,	-size,	-size);
		glTexCoord2f(0.0,1.0);		glVertex3f(size,	-size,	-size);
		glTexCoord2f(1.0,1.0);		glVertex3f(size,	size,	-size);
		glTexCoord2f(1.0,0.0);		glVertex3f(-size,	size,	-size);
	glEnd();
}

char* extractFilePath(char* fileName) {
	int	i, j;
	for ( i = strlen(fileName); i >= 0; i-- ) {
		if ( fileName[i] == '\\' ) {
			break;
		}
	}
	if ( i >= 0 ) {
		char	*s = (char*)malloc(sizeof(char) * i + 2);
		for ( j = 0; j <= i; j++ ) {
			s[j] = fileName[j];
		}
		s[i + 2] = 0;
		return s;
	} else {
		return "";
	}
}

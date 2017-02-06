#ifndef SNOOKERUTILS_H_INCLUDED
#define SNOOKERUTILS_H_INCLUDED

#include	<string.h>
#include	"SnookerDefs.h"

char	*currentDir;

typedef struct {
	TFloat	x, y;
} TVertex, *PVertex;

typedef struct {
	TFloat	x, y, z;
} TVertex3D, *PVertex3D;

typedef	struct {
	unsigned	x, y;
} TUVertex, *PUVertex;

typedef struct {
	TVertex	p1, p2;
} TLine, *PLine;

typedef struct {
	TFloat	i, j;
} TVector2D, *PVector2D;

inline	TFloat		radToDegree(TFloat rad);
inline	TFloat		sqr(TFloat x);
inline	TVertex		vertex(TFloat x, TFloat y);
inline	TUVertex	uvertex(unsigned x, unsigned y);
inline	TVertex3D	vertex3D(TFloat x, TFloat y, TFloat z);
inline	int			sameVertex(TVertex v1, TVertex v2);
inline	int			sameUVertex(TUVertex v1, TUVertex v2);
		TFloat		distancePointToPoint(TVertex p1, TVertex p2);
		TFloat		distancePointToPoint3D(TVertex3D p1, TVertex3D p2);
		TFloat		distancePointToLine(TVertex p, PLine l);
		TFloat		distancePointToLineSegment(TVertex p, PLine l);
inline	double		angularCoef(TVertex v1, TVertex v2);
		void		drawCircle2D(TFloat radius, TFloat xc, TFloat yc);
		void		drawCircle3Dxy(TFloat radius, TFloat xc, TFloat yc, TFloat z);
		void		drawWiredCircleSection2D(TFloat radius, TFloat xc, TFloat yc, TFloat startAngle, TFloat endAngle, TFloat slice);
		void		drawWiredCircleSection3Dxy(TFloat radius, TFloat xc, TFloat yc, TFloat startAngle, TFloat endAngle, TFloat slice, TFloat z);
		void		drawCircleSectionCustom3Dxy(TFloat radius, TFloat xc, TFloat yc, TFloat startAngle, TFloat endAngle, unsigned slices, TFloat z);
		void		drawTexturedCilinderSection(TFloat angle, unsigned slices, int normals);
		void		drawTexturedCube(TFloat size);
inline	TVector2D	angleToVector(TFloat angle);
inline	TFloat		vectorToAngle(PVector2D vector);
		TVector2D	vectorNormal(PVector2D vector);
inline	TFloat		vectorNorma(PVector2D v);
		TFloat		vectorMultEscalar(PVector2D a, PVector2D b);
inline	TVector2D	vectorEscalarMult(PVector2D v, TFloat n);
inline	TVector2D	vectorSum(PVector2D a, PVector2D b);
inline	TVector2D	vectorDiff(PVector2D a, PVector2D b);
		TVector2D	vectorReflect(PVector2D L, PVector2D N);
inline	TVector2D	vectorNegative(PVector2D v);
inline	TFloat		vectorAngleBetween(PVector2D a, PVector2D b);
inline	void		vectorNormalize(PVector2D v);
		TVertex		projectionPointOverLine(TVertex p, PLine l);
inline	int			equationSolve2(TFloat a, TFloat b, TFloat c, PFloat x1, PFloat x2);
inline	GLfloat*	p4ToGLfv(TFloat r, TFloat g, TFloat b, TFloat a, GLfloat *v);
		char*		extractFilePath(char* fileName);

#endif // SNOOKERUTILS_H_INCLUDED

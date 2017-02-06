#ifndef PROGRESSBAR_H_INCLUDED
#define PROGRESSBAR_H_INCLUDED

#include	"SnookerDefs.h"

typedef struct {
	unsigned	x, y, width, height;
	int			visible;
	TFloat		value;
	GLfloat		r, g, b;
} TProgressBar, *PProgressBar;

PProgressBar	progressBarCreate();
void			progressBarDestroy(PProgressBar *bar);
void			progressBarSetColor(PProgressBar bar, GLfloat r, GLfloat g, GLfloat b);
void			progressBarSetXY(PProgressBar bar, unsigned x, unsigned y);
void			progressBarSetDimension(PProgressBar bar, unsigned width, unsigned height);
void			progressBarSetVisible(PProgressBar bar, int visible);
TFloat			progressBarGetValue(PProgressBar bar);
void			progressBarAddValue(PProgressBar bar, TFloat value);
void			progressBarReset(PProgressBar bar);
void			progressBarPaint2D(PProgressBar bar);

#endif // PROGRESSBAR_H_INCLUDED

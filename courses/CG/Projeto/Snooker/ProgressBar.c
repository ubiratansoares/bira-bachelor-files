#include	"ProgressBar.h"

PProgressBar progressBarCreate() {
	PProgressBar	bar;
	bar = (PProgressBar)malloc(sizeof(TProgressBar));
	bar->height = 10;
	bar->width = 100;
	bar->visible = 0;
	bar->value = 0;
	bar->x = 0;
	bar->y = 0;
	return bar;
}

void progressBarDestroy(PProgressBar *bar) {
	free(*bar);
	*bar = 0;
}

void progressBarSetXY(PProgressBar bar, unsigned x, unsigned y) {
	bar->x = x;
	bar->y = y;
}

void progressBarSetDimension(PProgressBar bar, unsigned width, unsigned height) {
	bar->width = width;
	bar->height = height;
}

void progressBarSetVisible(PProgressBar bar, int visible) {
	bar->visible = visible;
}

TFloat progressBarGetValue(PProgressBar bar) {
	return bar->value;
}

void progressBarReset(PProgressBar bar) {
	bar->value = 0;
}

void progressBarAddValue(PProgressBar bar, TFloat value) {
	bar->value += value;
	if ( bar->value > 1.0 ) {
		bar->value = 1.0;
	}
}

void progressBarSetColor(PProgressBar bar, GLfloat r, GLfloat g, GLfloat b) {
	bar->r = r;
	bar->g = g;
	bar->b = b;
}

void progressBarPaint2D(PProgressBar bar) {
	if ( bar->visible ) {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glViewport(bar->x,bar->y,bar->width,bar->height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0,1.0,0.0,1.0,-1.0,1.0);

		glMatrixMode(GL_MODELVIEW);

		glColor3f(bar->r,bar->g,bar->b);
		glBegin(GL_QUADS);
			glVertex2f(0.0,			0.0);
			glVertex2f(0.0,			1.0);
			glVertex2f(bar->value,	1.0);
			glVertex2f(bar->value,	0.0);
		glEnd();
	}
}

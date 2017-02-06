/*
 * retas.c
 *
 *  Created on: Mar 3, 2009
 *      Author: tacito
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>

int count;

void traceLine(int x1, int y1, int x2, int y2) {
	int dx, dy, incE, incNE, d, x, y;

	dx = x2 - x1;
	dy = y2 - y1;
	d = 2 * dy - dx; 		/* Valor inicial de d */
	incE = 2 * dy; 			/* Incremento de E */
	incNE = 2 * (dy - dx); 	/* Incremento de NE */
	x = x1;
	y = y1;

	glBegin(GL_POINTS);
		glVertex2d(x, y);
	glEnd();

	while (x < x2) {
		if (d <= 0) {
			/* Escolhe E */
			d = d + incE;
			x = x + 1;
		} else {
			/* Escolhe NE */
			d = d + incNE;
			x = x + 1;
			y = y + 1;
		}

		glBegin(GL_POINTS);
			glVertex2d(x, y);
		glEnd();
	}
}

void redrawLine() {
	char buf[10];
    int i;
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor4f(1.0,0.0,0.0,1);
    glBegin(GL_POINTS);
		i = sprintf(buf, "%d", count++);
		glutSetWindowTitle((const char*)buf);
        glVertex2f(200.0,200.0);
    glEnd();
    traceLine(1,1,198,198);
    glFlush();
}

int main(int argc, char **argv) {
	count = 0;
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);

    glutInitWindowSize(400,400);
    glutInitWindowPosition(1,1);
    glutCreateWindow("Ponto 1");
    gluOrtho2D(0,199,0,199);
    glutDisplayFunc(redrawLine);

    glutMainLoop();

    return 0;
}

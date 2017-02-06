#include	<stdlib.h>
#include	<stdio.h>
#include	<unistd.h>
#include	<time.h>
#include	<OpenGL/gl.h>
#include	<OpenGL/glu.h>
#include	<GLUT/glut.h>
#include	"Snooker.h"

#define	M_TABLE_WIDTH			3.569
#define	M_TABLE_HEIGHT			1.778
#define	M_TABLE_TOTAL_WIDTH		3.769
#define	M_TABLE_TOTAL_HEIGHT	1.978
#define	M_BALLS					1
#define	M_BALL_RADIUS			0.02625
#define	M_BALL_MASS				1.0
#define	M_WHITE_BALL_RADIUS		0.034
#define	M_WHITE_BALL_MASS		1.0

float		timeRatio = 1.0;
PSnooker	snooker;
unsigned	fps = 0;
float		nowTime = 0, lastTime = 0, timeElapsed = 0, timeFPS = 0;

void resize(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,M_TABLE_TOTAL_WIDTH,0,M_TABLE_TOTAL_HEIGHT,0,1);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,width,height);
}

void display(void) {

	nowTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	timeElapsed = nowTime - lastTime;
	lastTime = nowTime;
	timeFPS += timeElapsed;
	fps++;

	if ( !snookerIsPaused(snooker) ) {
		if ( timeFPS > 1.0 ) {
			printf("\n> FPS: %.1f\n",(float)fps / timeFPS);
			timeFPS = 0;
			fps = 0;
		}
		snookerThink(snooker,timeElapsed / timeRatio);
	}

	// rendering.
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	snookerPaint(snooker);

	glutSwapBuffers();
}


void key(unsigned char key, int x, int y) {
	if ( key == ' ' ) {
		snookerPause(snooker,!snookerIsPaused(snooker));
	} else if ( key == 's' ) {
		timeRatio += 0.1;
		printf("\n> timeRatio=%.1f\n",timeRatio);
	} else if ( key == 'f' ) {
		if ( timeRatio > 0.1 ) {
			timeRatio -= 0.1;
		}
		printf("\n> timeRatio=%.1f\n",timeRatio);
	}
}

void idle(void) {
	if ( snookerIsPaused(snooker) ) {
		usleep(50000);
	}
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {

	#ifndef DISABLE_RANDSEED
		srand(time(0));
	#endif

    glutInit(&argc, argv);
    glutInitWindowSize(1000,486);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutCreateWindow("Snooker");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(0,0,0,0);

	snooker = snookerCreate();
	snookerSetupTable(snooker,M_TABLE_TOTAL_WIDTH,M_TABLE_TOTAL_HEIGHT,M_TABLE_WIDTH,M_TABLE_HEIGHT,M_WHITE_BALL_RADIUS * 2.2,0.01);
	snookerSetupBalls(snooker,M_BALLS,M_BALL_RADIUS,M_BALL_MASS);
	snookerSetupWhiteBall(snooker,M_WHITE_BALL_RADIUS,M_WHITE_BALL_MASS);
	snookerSetBallFlag(snooker,-1,BALL_DRAW_SPEED_VECTOR);

	ballApplyMovement(snookerGetWhiteBall(snooker),M2_PI * rand() / (float)RAND_MAX,1.0);
//	ballApplyMovement(snookerGetWhiteBall(snooker),0,3.0);

	// Enable AA.
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glutMainLoop();

    return EXIT_SUCCESS;
}

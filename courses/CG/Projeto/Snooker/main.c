#include	<stdlib.h>
#include	<stdio.h>
#include	<unistd.h>
#include	<time.h>
#include	<OpenGL/gl.h>
#include	<OpenGL/glu.h>
#include	<GLUT/glut.h>
#include	"Snooker.h"
#include	"ProgressBar.h"

#define	D_WINDOW_WIDTH				1000
#define	D_WINDOW_HEIGHT				486

#define D_LAYOUT_RIGHTBAR_WIDTH		300

#define	D_MAX_TACKLE				4.0
#define	D_MAX_TACKLE_TIME			3.0

#define	D_STATE_STOPPED				0
#define	D_STATE_PREPARING			1
#define	D_STATE_RUNNING				2
#define	D_STATE_ENDED				3
#define	D_STATE_STARTING			4

float			timeRatio = 1.0;
float			angleRatio = 1.0;
PSnooker		snooker;
PProgressBar	bar;
unsigned		fps = 0, state = D_STATE_STARTING;
float			nowTime = 0, lastTime = 0, timeElapsed = 0, timeFPS = 0;

void resetLastTime() {
	lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
}

void resize(int width, int height) {
	snookerSetTable2DBounds(snooker,uvertex(width - D_LAYOUT_RIGHTBAR_WIDTH,height - D_LAYOUT_RIGHTBAR_WIDTH / (int)D_TABLE_ASPECT),uvertex(D_LAYOUT_RIGHTBAR_WIDTH,D_LAYOUT_RIGHTBAR_WIDTH / (int)D_TABLE_ASPECT));
	snookerSetTable3DBounds(snooker,uvertex(0,0),uvertex(width,height));
	progressBarSetDimension(bar,width,20);
	progressBarSetXY(bar,0,0);
}

void display(void) {

	// fps computing.
	nowTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	timeElapsed = nowTime - lastTime;
	lastTime = nowTime;
	timeFPS += timeElapsed;
	fps++;

//	if ( timeElapsed > 0.05 ) {
//		timeElapsed = 0.05;
//	}

	// camera pre-processing.
	snookerCameraUpdate(snooker);

	// game process.
	if ( state == D_STATE_PREPARING ) {
		progressBarAddValue(bar,timeElapsed * 1.0 / D_MAX_TACKLE_TIME);
	} else if ( state == D_STATE_RUNNING && !snookerIsPaused(snooker) ) {
//		printf("-- %.3f\n",timeElapsed);
		if ( timeFPS > 1.0 ) {
			timeFPS = 0;
			fps = 0;
		}
		snookerThink(snooker,timeElapsed / timeRatio);
		if ( !snookerMovementInProgress(snooker) ) {
			if ( snookerGameEnded(snooker) ) {
				state = D_STATE_ENDED;
				printf("> Game Over!\n");
			} else {
				state = D_STATE_STOPPED;
				snookerSetCameraD(snooker,D_CAMERA_DISTANCE);
				snookerSetCameraZC(snooker,D_CAMERA_HEIGHT);
				snookerSetCameraMode(snooker,D_CAMERA_MODE_CENTERED);
				snookerAddCameraAngle(snooker,0.0);
//				glutPostRedisplay();
			}
		}
	}

	// rendering.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	snookerPaint3D(snooker);
	snookerPaint2D(snooker);
	progressBarPaint2D(bar);

	glutSwapBuffers();
}

void key(unsigned char key, int x, int y) {
	if ( key == 'l' ) {
		if ( state == D_STATE_STOPPED ) {
			state = D_STATE_STARTING;
			snookerSetCameraMode(snooker,D_CAMERA_MODE_FAR);
			snookerCameraUpdate(snooker);
			glutPostRedisplay();
		} else if ( state == D_STATE_STARTING ) {
			state = D_STATE_STOPPED;
			snookerSetCameraMode(snooker,D_CAMERA_MODE_CENTERED);
			snookerCameraUpdate(snooker);
			snookerSetCameraD(snooker,D_CAMERA_DISTANCE);
			snookerSetCameraZC(snooker,D_CAMERA_HEIGHT);
			snookerAddCameraAngle(snooker,0.0);
			glutPostRedisplay();
		}
		return;
	} else if ( key == ' ' ) {
		if ( snookerIsPaused(snooker) ) {
			snookerPause(snooker,0);
		} else {
			snookerPause(snooker,1);
		}
		lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	} else if ( key == 'r' ) {
//		timeRatio += 0.1;
	} else if ( key == 't' ) {
//		if ( timeRatio > 0.1 ) {
//			timeRatio -= 0.1;
//		}
	} else if ( key == 'd' && state == D_STATE_STOPPED ) {
		snookerAddCameraAngle(snooker,angleRatio*(D_PI / 360.0));
	} else if ( key == 'a' && state == D_STATE_STOPPED ) {
		snookerAddCameraAngle(snooker,-angleRatio*(D_PI / 360.0));
	} else if ( key == 'x' && state == D_STATE_STOPPED ) {
		angleRatio*= 2.0;
	} else if ( key == 'z' && state == D_STATE_STOPPED ) {
		angleRatio/= 2.0;
	} else if ( key == 'p' && state == D_STATE_STOPPED ) {
		if ( ballGetFlag(snookerGetCueBall(snooker),BALL_DRAW_SPEED_VECTOR) ) {
			snookerClearBallFlag(snooker,-1,BALL_DRAW_SPEED_VECTOR);
		} else {
			snookerSetBallFlag(snooker,-1,BALL_DRAW_SPEED_VECTOR);
		}
	} else {
		if ( key == 'q' ) {
			if ( state == D_STATE_PREPARING ) {
				state = D_STATE_RUNNING;
				snookerTackle(snooker,progressBarGetValue(bar) * D_MAX_TACKLE);
//				printf("snookerTackle=%.3f\n",progressBarGetValue(bar) * D_MAX_TACKLE);
				snookerSetCameraMode(snooker,D_CAMERA_MODE_WATCH);
				snookerCameraUpdate(snooker);
				snookerSetCameraZC(snooker,D_TABLE_FLOOR_HEIGHT * 1.4);
				progressBarSetVisible(bar,0);
			} else if ( state == D_STATE_STOPPED ) {
				state = D_STATE_PREPARING;
				progressBarReset(bar);
				progressBarSetVisible(bar,1);
			}
		}
	}
	glutPostRedisplay();
	resetLastTime();
}

void idle(void) {
	if ( state == D_STATE_PREPARING ) {
		usleep(10000);
		glutPostRedisplay();
	} else if ( snookerIsPaused(snooker) ) {
		usleep(50000);
	} else if ( !snookerMovementInProgress(snooker) ) {
		usleep(20000);
		resetLastTime();
		glutPostRedisplay();
	} else {
		usleep(1000);
		glutPostRedisplay();
	}
}

int main(int argc, char *argv[]) {

	currentDir = extractFilePath(argv[0]);

	#ifdef	DEBUG
		printf("* ATENCAO! DEPURACAO!\n");
		printf("  A depuracao podera reduzir significavelmente o desempenho do jogo.\n");
	#endif

	printf("> Keys:\n");
//	printf("  r       reduz a velocidade do tempo.\n");
//	printf("  t       aumenta a velocidade do tempo.\n");
	printf("  p       liga/desliga o desenho dos vetores velocidade.\n");
	printf("  espaco  pause.\n");
	printf("  a       gira a direcao a camera no sentido horario\n");
	printf("  d       gira a direcao a camera no sentido anti-horario\n");
	printf("  x       aumenta a variacao do angulo do giro\n");
	printf("  z       diminui a variacao do angulo do giro\n");
//	printf("  o       tacada aleatoria\n");
//	printf("  i       tacada na bola mais distante da bola branca.\n");
//	printf("  u       tacada na bola mais proxima da bola branca.\n");
//	printf("  y       tacada em uma bola aleatoria.\n");
	printf("  q       tacada na direcao da camera.\n\n");

	#ifndef DISABLE_RANDSEED
		srand(time(0));
	#endif

	bar = progressBarCreate();
	progressBarSetColor(bar,1.0,0.0,0.0);

    glutInit(&argc, argv);
    glutInitWindowSize(D_WINDOW_WIDTH,D_WINDOW_HEIGHT);
    glutInitWindowPosition(0,0);
    //glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA | GLUT_MULTISAMPLE); 
	

    glutCreateWindow("Snooker");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(0,0,0,0);

	// prepare the game.
	snooker = snookerCreate();
	snookerSetupTable(snooker,D_TABLE_WIDTH + D_TABLE_BORDER,D_TABLE_HEIGHT + D_TABLE_BORDER,D_TABLE_WIDTH,D_TABLE_HEIGHT,D_TABLE_POCKET_DIAMETER,0.02);
	snookerSetupBalls(snooker,D_BALLS,D_BALL_RADIUS,D_BALL_MASS);
	snookerSetupCueBall(snooker,D_CUEBALL_RADIUS,D_CUEBALL_MASS);
	snookerSetupOfficialBallColors(snooker);
	snookerPause(snooker,0);

	// prepare clear color.
	glClearColor(0.0,0.0,0.0,0.0);

	snookerSetCameraMode(snooker,D_CAMERA_MODE_FAR);
	snookerCameraUpdate(snooker);
	snookerSetCameraD(snooker,D_CAMERA_DISTANCE);
	snookerSetCameraZC(snooker,D_CAMERA_HEIGHT);
	snookerSetCameraAngle(snooker,0.0);

	//glEnable(GL_DEPTH_TEST);
	
    glutMainLoop();

    return EXIT_SUCCESS;
}

#include	"Snooker.h"

#define		D_CUEBALL	0

PSnooker snookerCreate() {
	PSnooker snooker = malloc(sizeof(TSnooker));
	snooker->paused = 1;
	snookerSetCameraMode(snooker,D_CAMERA_MODE_FREE);
	snookerSetCameraXC(snooker,0.0);
	snookerSetCameraYC(snooker,0.0);
	snookerSetCameraZC(snooker,D_TABLE_FLOOR_HEIGHT + 0.1);
	snookerSetCameraXV(snooker,1.0);
	snookerSetCameraYV(snooker,1.0);
	snookerSetCameraZV(snooker,D_TABLE_FLOOR_HEIGHT);
	snookerSetCameraD(snooker,D_CAMERA_DISTANCE);

	char	fileName[1024];

	fileName[0] = 0;
	snooker->wall = textureCreate(strcat(strcpy(fileName,currentDir),"wall.raw"));
	glGenTextures(1,&(snooker->wall->glID));
	glBindTexture(GL_TEXTURE_2D,snooker->wall->glID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,snooker->wall->width,snooker->wall->height,0,GL_RGB,GL_UNSIGNED_BYTE,snooker->wall->pixels);
	textureRelease(snooker->wall);

	fileName[0] = 0;
	snooker->floor = textureCreate(strcat(strcpy(fileName,currentDir),"floor.raw"));
	glGenTextures(1,&(snooker->floor->glID));
	glBindTexture(GL_TEXTURE_2D,snooker->floor->glID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,snooker->floor->width,snooker->floor->height,0,GL_RGB,GL_UNSIGNED_BYTE,snooker->floor->pixels);
	textureRelease(snooker->floor);

	return snooker;
}

void snookerSetupTable(PSnooker snooker, TFloat totalWidth, TFloat totalHeight, TFloat width, TFloat height,
TFloat pocketSize, TFloat atrCoef) {
	snooker->table = tableCreate(totalWidth,totalHeight);
	tableSetup(snooker->table,width,height,pocketSize,atrCoef);
}

void snookerSetupBalls(PSnooker snooker, unsigned balls, TFloat radius, TFloat mass) {
	snooker->balls = balls;
	snooker->ball = (PBall*)malloc(sizeof(PBall) * balls);
	unsigned	i = 0;
	for ( i = 0; i < balls; i++ ) {
		snooker->ball[i] = ballCreate("");
		ballSetup(snooker->ball[i],radius,mass);
		float	r = rand() / (TFloat)RAND_MAX, g = rand() / (TFloat)RAND_MAX, b = rand() / (TFloat)RAND_MAX;
		ballSetColor2D(snooker->ball[i],r,g,b);
		ballSetColor3D(snooker->ball[i],r,g,b);
		ballSetXY(snooker->ball[i],(i + 1) * snooker->table->width / (balls + 1),snooker->table->height / 2.0);
		ballApplyMovement(snooker->ball[i],0.0,0.0);
	}
}

void snookerSetupCueBall(PSnooker snooker, TFloat radius, TFloat mass) {
	ballSetup(snookerGetCueBall(snooker),radius,mass);
	ballSetColor2D(snookerGetCueBall(snooker),1.0,1.0,1.0);
	ballSetColor3D(snookerGetCueBall(snooker),1.0,1.0,1.0);
}

void snookerDestroy(PSnooker *snkr) {
	PSnooker snooker = *snkr;
	int	i;
	for ( i = 0; i < snooker->balls; i++ ) {
		ballDestroy(&(snooker->ball[i]));
	}
	free(snooker->ball);
	tableDestroy(&(snooker->table));
	free(snooker);
	*snkr = 0;
}

void snookerSetTable2DBounds(PSnooker snooker, TUVertex p, TUVertex d) {
	snooker->position2D = p;
	snooker->dimension2D = d;
}

void snookerSetTable3DBounds(PSnooker snooker, TUVertex p, TUVertex d) {
	snooker->position3D = p;
	snooker->dimension3D = d;
}

PBall snookerGetCueBall(PSnooker snooker) {
	return snooker->ball[D_CUEBALL];
}

unsigned snookerGetBalls(PSnooker snooker) {
	return snooker->balls;
}

PBall snookerGetBall(PSnooker snooker, unsigned ball) {
	return snooker->ball[ball];
}

void snookerPaint2D(PSnooker snooker) {
	// enable AA.
//	glEnable(GL_LINE_SMOOTH);
//	glEnable(GL_POLYGON_SMOOTH);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	// prepare the projection and the modelview.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(snooker->position2D.x,snooker->position2D.y,snooker->dimension2D.x,snooker->dimension2D.y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,tableGetTotalWidth(snooker->table),0.0,tableGetTotalHeight(snooker->table),-1.0,1.0);

	glMatrixMode(GL_MODELVIEW);

	// paint.
	unsigned	i;
	TFloat		border = tableGetBorder(snooker->table);
	tablePaint2D(snooker->table);
	for ( i = 0; i < snooker->balls; i++ ) {
		if ( !ballGetFlag(snooker->ball[i],BALL_DEAD) ) {
			ballPaint2D(snooker->ball[i],border);
		}
	}

	// paint the direction vector.
	if ( !snookerMovementInProgress(snooker) ) {
		TFloat		sx, sy, ex, ey, a;
		TVector2D	v;
		sx = ballGetX(snookerGetCueBall(snooker)) + border;
		sy = ballGetY(snookerGetCueBall(snooker)) + border;
		v.i = snooker->camera.xv - snooker->camera.xc;
		v.j = snooker->camera.yv - snooker->camera.yc;
		a = vectorToAngle(&v);
		ex = cos(a) + sx;
		ey = sin(a) + sy;
		glColor3f(0.0,1.0,0.0);
		glBegin(GL_LINES);
			glVertex2f(sx,sy);
			glVertex2f(ex,ey);
		glEnd();
	}

	// disable AA.
//	glDisable(GL_LINE_SMOOTH);
//	glDisable(GL_POLYGON_SMOOTH);
//	glDisable(GL_BLEND);
}

void snookerPaintScenario3D(PSnooker snooker) {
	GLfloat	white[4], black[4];

	p4ToGLfv(1.0,1.0,1.0,1.0,white);
	p4ToGLfv(0.0,0.0,0.0,1.0,black);

	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT,GL_AMBIENT,white);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,white);
	glMaterialfv(GL_FRONT,GL_SPECULAR,white);
	glMaterialf (GL_FRONT,GL_SHININESS,0);
	//glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	//glBindTexture(GL_TEXTURE_2D,snooker->wall->glID);
	glBegin(GL_QUADS);
		// walls.
		//glNormal3f(-1.0,0.0,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(6.0,		4.0,	0.0);
		glTexCoord2f(2.0,0.0);
		glVertex3f(6.0,		-2.0,	0.0);
		glTexCoord2f(2.0,1.0);
		glVertex3f(6.0,		-2.0,	2.0);
		glTexCoord2f(0.0,1.0);
		glVertex3f(6.0,		4.0,	2.0);

		//glNormal3f(0.0,-1.0,0.0);
		glTexCoord2f(2.0,0.0);
		glVertex3f(6.0,		4.0,	0.0);
		glTexCoord2f(2.0,1.0);
		glVertex3f(6.0,		4.0,	2.0);
		glTexCoord2f(0.0,1.0);
		glVertex3f(-2.0,	4.0,	2.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(-2.0,	4.0,	0.0);

		//glNormal3f(1.0,0.0,0.0);
		glTexCoord2f(2.0,0.0);
		glVertex3f(-2.0,	4.0,	0.0);
		glTexCoord2f(2.0,1.0);
		glVertex3f(-2.0,	4.0,	2.0);
		glTexCoord2f(0.0,1.0);
		glVertex3f(-2.0,	-2.0,	2.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(-2.0,	-2.0,	0.0);

		//glNormal3f(0.0,1.0,0.0);
		glTexCoord2f(2.0,0.0);
		glVertex3f(6.0,		-2.0,	0.0);
		glTexCoord2f(2.0,1.0);
		glVertex3f(6.0,		-2.0,	2.0);
		glTexCoord2f(0.0,1.0);
		glVertex3f(-2.0,	-2.0,	2.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(-2.0,	-2.0,	0.0);
	glEnd();

	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT,GL_AMBIENT,white);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,white);
	glMaterialfv(GL_FRONT,GL_SPECULAR,white);
	glMaterialf (GL_FRONT,GL_SHININESS,0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,snooker->floor->glID);
	glBegin(GL_QUADS);
		// ground.
		glNormal3f(0.0,0.0,1.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(-2.0,	-2.0,	0.0);
		glTexCoord2f(4.0,0.0);
		glVertex3f(6.0,		-2.0,	0.0);
		glTexCoord2f(4.0,3.0);
		glVertex3f(6.0,		4.0,	0.0);
		glTexCoord2f(0.0,3.0);
		glVertex3f(-2.0,	4.0,	0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT,GL_AMBIENT,white);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,black);
	glMaterialfv(GL_FRONT,GL_SPECULAR,black);
	glMaterialf (GL_FRONT,GL_SHININESS,0);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,snooker->floor->glID);
	glBegin(GL_QUADS);
		// roof.
		glNormal3f(0.0,0.0,-1.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(-2.0,	-2.0,	2.0);
		glTexCoord2f(4.0,0.0);
		glVertex3f(6.0,		-2.0,	2.0);
		glTexCoord2f(4.0,3.0);
		glVertex3f(6.0,		4.0,	2.0);
		glTexCoord2f(0.0,3.0);
		glVertex3f(-2.0,	4.0,	2.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void snookerPaint3D(PSnooker snooker) {

	glEnable(GL_DEPTH_TEST);

	// prepare the projection and the modelview.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(snooker->position3D.x,snooker->position3D.y,snooker->dimension3D.x,snooker->dimension3D.y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0,(TFloat)snooker->dimension3D.x / (TFloat)snooker->dimension3D.y,0.1,20.0);

	glMatrixMode(GL_MODELVIEW);

//	gluLookAt(-1.0,-1.0,1.0,
//		0.0,0.0,0.5,
//		0.0,0.0,1.0);

//	gluLookAt(-0.5,-0.5,1.0,
//		1.2,0.8,0.4,
//		0.0,0.0,1.0);

//	gluLookAt(1.8,0.9,4.0,
//		1.8,0.9,0.0,
//		0.0,1.0,0.0);

//	gluLookAt(1.8,0.0,3.0,
//		1.8,0.0,0.0,
//		0.0,1.0,0.0);

//	gluLookAt(0.3,0.3,1.0,
//		0.0,0.0,0.9,
//		0.0,0.0,1.0);

	gluLookAt(snooker->camera.xc,snooker->camera.yc,snooker->camera.zc,
		snooker->camera.xv,snooker->camera.yv,snooker->camera.zv,
		0.0,0.0,1.0);

	GLfloat	v[4];

	glLightfv(GL_LIGHT0,GL_POSITION,p4ToGLfv(tableGetTotalWidth(snooker->table) / 4.0,tableGetTotalHeight(snooker->table) / 2.0,2.0,1.0,v));
	glLightfv(GL_LIGHT0,GL_DIFFUSE,p4ToGLfv(1.0,1.0,1.0,1.0,v));
	glLightfv(GL_LIGHT0,GL_SPECULAR,p4ToGLfv(1.0,1.0,1.0,1.0,v));
	glLightfv(GL_LIGHT0,GL_AMBIENT,p4ToGLfv(0.0,0.0,0.0,1.0,v));
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,p4ToGLfv(0.0,0.0,-1.0,1.0,v));
	glLightf (GL_LIGHT0,GL_SPOT_CUTOFF,60.0);
	glLightf (GL_LIGHT0,GL_SPOT_EXPONENT,1.0);
	glLightf (GL_LIGHT0,GL_CONSTANT_ATTENUATION,0.0);
	glLightf (GL_LIGHT0,GL_LINEAR_ATTENUATION,0.0);
	glLightf (GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.5);

	glLightfv(GL_LIGHT1,GL_POSITION,p4ToGLfv(3 * tableGetTotalWidth(snooker->table) / 4.0,tableGetTotalHeight(snooker->table) / 2.0,2.0,1.0,v));
	glLightfv(GL_LIGHT1,GL_DIFFUSE,p4ToGLfv(1.0,1.0,1.0,1.0,v));
	glLightfv(GL_LIGHT1,GL_SPECULAR,p4ToGLfv(1.0,1.0,1.0,1.0,v));
	glLightfv(GL_LIGHT1,GL_AMBIENT,p4ToGLfv(0.0,0.0,0.0,1.0,v));
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,p4ToGLfv(0.0,0.0,-1.0,1.0,v));
	glLightf (GL_LIGHT0,GL_SPOT_CUTOFF,60.0);
	glLightf (GL_LIGHT0,GL_SPOT_EXPONENT,1.0);
	glLightf (GL_LIGHT0,GL_CONSTANT_ATTENUATION,0.0);
	glLightf (GL_LIGHT0,GL_LINEAR_ATTENUATION,0.0);
	glLightf (GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.5);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,p4ToGLfv(0.3,0.3,0.3,1.0,v));

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	// paint.
	unsigned	i;
	TFloat		border = tableGetBorder(snooker->table);
	snookerPaintScenario3D(snooker);
	tablePaint3D(snooker->table,ballGetRadius(snookerGetCueBall(snooker)));
	glEnable(GL_DEPTH_TEST);
	for ( i = 0; i < snooker->balls; i++ ) {
		if ( !ballGetFlag(snooker->ball[i],BALL_DEAD) ) {
			ballPaint3D(snooker->ball[i],border);
		}
	}

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHTING);

	glDisable(GL_DEPTH_TEST);
}

void snookerStepBalls(PSnooker snooker, TFloat time) {
	#ifdef DEBUG
		printf("\nsnookerStepBalls(snooker,%.6f)",time);
	#endif
	int		i;
	TFloat	m = tableGetAtrCoef(snooker->table);
	for ( i = 0; i < snooker->balls; i++ ) {
		if ( !ballGetFlag(snooker->ball[i],BALL_DEAD) ) {
			#ifdef DEBUG
				printf("  %d ",i);
			#endif
			ballStep(snooker->ball[i],m,time);
		}
	}
}

void snookerThink(PSnooker snooker, TFloat timeElapsed) {

	if ( timeElapsed <= 0.0 ) {
		return;
	}

	if ( timeElapsed > D_SNOOKER_THINK_MAX_TIME_SLICE ) {
		#ifdef DEBUG
			puts("- Splitting timeElapsed!");
		#endif
		snookerThink(snooker,timeElapsed - D_SNOOKER_THINK_MAX_TIME_SLICE);
		timeElapsed = D_SNOOKER_THINK_MAX_TIME_SLICE;
		#ifdef DEBUG
			puts("- Split Done!");
		#endif
	}

	PTable	table = snooker->table;
	PBall	*ball = snooker->ball;
	int		balls = snooker->balls, i, j, hit;

	do {

		#ifdef DEBUG
			printf("\n> snookerThink(snooker,%.6f)\n",timeElapsed);
		#endif

		// move balls to the position so we can calculate if any ball collided with anything.
		snookerStepBalls(snooker,timeElapsed);

		#ifdef DEBUG
			puts("\n> Testing Hit");
		#endif

		hit = 0;

		TFloat		time = 0.0;
		int			hitWithTable = 1, hitTableBound = 0;
		unsigned	hitBall1 = 0, hitBall2 = 0;

		// check if any ball collided with the table.
		for ( i = 0; i < balls; i++ ) {
			if ( !ballGetFlag(ball[i],BALL_DEAD) && ballSpeed(ball[i]) != 0 ) {
				// a collision happened? How was this collision?
				int	tempHitTableBound = tableCollided(table,ball[i]);
				if ( tempHitTableBound ) {
					#ifdef DEBUG
						puts("  - Collision Detected");
						printf("    ball=%u hitTableBound=%u X=%.6f Y=%.6f R=%.6f\n",i,tempHitTableBound,ball[i]->x,ball[i]->y,ball[i]->radius);
					#endif
					// get the actual time of the collision.
					TFloat	timeTemp = tableCollisionTime(table,ball[i],tempHitTableBound);
					if ( timeTemp > time ) {
						time = timeTemp;
						hitTableBound = tempHitTableBound;
						hitWithTable = 1;
						hitBall1 = i;
						hit = 1;
						#ifdef DEBUG
							puts("  - Previous Collision Detected: Table");
							printf("    time=%.6f ball=%u hitTableType=%u X=%.6f Y=%.6f R=%.6f\n",time,hitBall1,hitTableBound,ball[i]->x,ball[i]->y,ball[i]->radius);
						#endif
					}
				}
			}
		}

		// check if collided with another ball.
		for ( i = 0; i < balls - 1; i++ ) {
			if ( !ballGetFlag(ball[i],BALL_DEAD) ) {
				for ( j = i + 1; j < balls; j++ ) {
					if ( !ballGetFlag(ball[j],BALL_DEAD) ) {
						// a collision happened?
						if ( ballCollided(ball[i],ball[j]) ) {
							// get the actual time of the collision.
							TFloat	timeTemp = ballCollisionTime(ball[i],ball[j]);
							if ( timeTemp > time ) {
								time = timeTemp;
								hitWithTable = 0;
								hitBall1 = i;
								hitBall2 = j;
								hit = 1;
								#ifdef DEBUG
									puts("  - Collision Detected: Balls");
									printf("    time=%.6f balls=(%u,%u)\n",time,hitBall1,hitBall2);
								#endif
							}
						}
					}
				}
			}
		}

		// if anything collided.
		if ( hit ) {
			// re-calculate the timeElapsed.
			timeElapsed -= time;
			#ifdef DEBUG
				if ( !hitWithTable ) {
					printf("  D=%.3f\n",ballDistance(snooker->ball[hitBall1],snooker->ball[hitBall2]));
				}
			#endif
			// step every ball to the exactly moment the collision happened.
			snookerStepBalls(snooker,-time);
			#ifdef DEBUG
				printf("  time=%.6f timeElapsed=%.6f\n",time,timeElapsed);
			#endif
			// apply the collision.
			if ( hitWithTable ) {
				if ( tableInPocket(snooker->table,snooker->ball[hitBall1],hitTableBound) ) {
					#ifdef	DEBUG
						printf("- Pocket Hit: %u\n",hitBall1);
					#endif
					ballSetFlag(snooker->ball[hitBall1],BALL_DEAD);
//					printf("pocket: ball=%u hit=%u\n",hitBall1,hitTableBound & table->pockets);
				} else {
					#ifdef DEBUG
						puts("- Table Hit");
						printf("  B=%u X=%.6f Y=%.6f A=%.3f V=%.3f\n",hitBall1,snooker->ball[hitBall1]->x,snooker->ball[hitBall1]->y,snooker->ball[hitBall1]->angle,snooker->ball[hitBall1]->speed);
					#endif
					tableCollision(snooker->table,snooker->ball[hitBall1],hitTableBound);
					#ifdef DEBUG
						printf("  A=%.3f V=%.3f\n",snooker->ball[hitBall1]->angle,snooker->ball[hitBall1]->speed);
					#endif
				}
			} else {
				#ifdef DEBUG
					puts("- Ball Hit");
					printf("  D=%.3f\n",ballDistance(snooker->ball[hitBall1],snooker->ball[hitBall2]));
					printf("  B=%u X=%.3f Y=%.3f A=%.3f V=%.3f\n",hitBall1,snooker->ball[hitBall1]->x,snooker->ball[hitBall1]->y,snooker->ball[hitBall1]->angle,snooker->ball[hitBall1]->speed);
					printf("  B=%u X=%.3f Y=%.3f A=%.3f V=%.3f\n",hitBall2,snooker->ball[hitBall2]->x,snooker->ball[hitBall2]->y,snooker->ball[hitBall2]->angle,snooker->ball[hitBall2]->speed);
				#endif
				ballCollision(snooker->ball[hitBall1],snooker->ball[hitBall2]);
				#ifdef DEBUG
					printf("  B=%u X=%.3f Y=%.3f A=%.3f V=%.3f\n",hitBall1,snooker->ball[hitBall1]->x,snooker->ball[hitBall1]->y,snooker->ball[hitBall1]->angle,snooker->ball[hitBall1]->speed);
					printf("  B=%u X=%.3f Y=%.3f A=%.3f V=%.3f\n",hitBall2,snooker->ball[hitBall2]->x,snooker->ball[hitBall2]->y,snooker->ball[hitBall2]->angle,snooker->ball[hitBall2]->speed);
				#endif
			}
		}

	} while ( hit );

}

int snookerIsPaused(PSnooker snooker) {
	return snooker->paused;
}

void snookerPause(PSnooker snooker, int pause) {
	snooker->paused = pause;
	if ( !snooker->paused ) {
		int	i;
		for ( i = 0; i < snooker->balls; i++ ) {
			ballClearFlag(snooker->ball[i],BALL_DRAW_ERROR_SIGNAL);
		}
	}
}

void snookerSetBallFlag(PSnooker snooker, int ball, unsigned flag) {
	if ( ball >= 0 ) {
		ballSetFlag(snooker->ball[ball],flag);
	} else {
		for ( ball = 0; ball < snooker->balls; ball++ ) {
			ballSetFlag(snooker->ball[ball],flag);
		}
	}
}

void snookerClearBallFlag(PSnooker snooker, int ball, unsigned flag) {
	if ( ball >= 0 ) {
		ballClearFlag(snooker->ball[ball],flag);
	} else {
		for ( ball = 0; ball < snooker->balls; ball++ ) {
			ballClearFlag(snooker->ball[ball],flag);
		}
	}
}

int snookerMovementInProgress(PSnooker snooker) {
	int	i, prog = 0;
	for ( i = 0; i < snooker->balls; i++ ) {
		if ( ballSpeed(snooker->ball[i]) != 0.0 && !ballGetFlag(snooker->ball[i],BALL_DEAD) ) {
			prog = 1;
		}
	}
	return prog;
}

int snookerGameEnded(PSnooker snooker) {
	int	i;
	for ( i = 0; i < snooker->balls; i++ ) {
		if ( snooker->ball[i] != snookerGetCueBall(snooker) && !ballGetFlag(snooker->ball[i],BALL_DEAD) ) {
			return 0;
		}
	}
	return 1;
}

void snookerTackleRandomBall(PSnooker snooker, TFloat speed) {
	unsigned	iball = 0;
	PBall		wball = snookerGetCueBall(snooker), ball;
	do {
		iball = rand() % snooker->balls;
		ball = snooker->ball[iball];
	} while ( ball == wball && ballGetFlag(ball,BALL_DEAD) );
	TFloat	angle = atan(angularCoef(vertex(wball->x,wball->y),vertex(ball->x,ball->y)));
	if ( ball->x < wball->x ) {
		angle += D_PI;
	}
	if ( angle < 0 ) {
		angle += D_2_PI;
	} else if ( angle > D_2_PI ) {
		angle -= D_2_PI;
	}
	ballApplyMovement(wball,angle,speed);
}

void snookerTackleClosestBall(PSnooker snooker, TFloat speed) {
	unsigned	i, iball = 0;
	TFloat		d = 1000.0, t;
	PBall		wball = snookerGetCueBall(snooker), ball;
	for ( i = 0; i < snooker->balls; i++ ) {
		ball = snooker->ball[i];
		if ( ball != wball && !ballGetFlag(ball,BALL_DEAD) ) {
			t = distancePointToPoint(vertex(ball->x,ball->y),vertex(wball->x,wball->y));
			if ( t < d ) {
				d = t;
				iball = i;
			}
		}
	}
	ball = snooker->ball[iball];
	TFloat	angle = atan(angularCoef(vertex(wball->x,wball->y),vertex(ball->x,ball->y)));
	if ( ball->x < wball->x ) {
		angle += D_PI;
	}
	if ( angle < 0 ) {
		angle += D_2_PI;
	} else if ( angle > D_2_PI ) {
		angle -= D_2_PI;
	}
	ballApplyMovement(wball,angle,speed);
}

void snookerTackleFartestBall(PSnooker snooker, TFloat speed) {
	unsigned	i, iball = 0;
	TFloat		d = 0, t;
	PBall		wball = snookerGetCueBall(snooker), ball;
	for ( i = 0; i < snooker->balls; i++ ) {
		ball = snooker->ball[i];
		if ( ball != wball && !ballGetFlag(ball,BALL_DEAD) ) {
			t = distancePointToPoint(vertex(ball->x,ball->y),vertex(wball->x,wball->y));
			if ( t > d ) {
				d = t;
				iball = i;
			}
		}
	}
	ball = snooker->ball[iball];
	TFloat	angle = atan(angularCoef(vertex(wball->x,wball->y),vertex(ball->x,ball->y)));
	if ( ball->x < wball->x ) {
		angle += D_PI;
	}
	if ( angle < 0 ) {
		angle += D_2_PI;
	} else if ( angle > D_2_PI ) {
		angle -= D_2_PI;
	}
	ballApplyMovement(wball,angle,speed);
}

void snookerTackleRandom(PSnooker snooker, TFloat speed) {
	ballApplyMovement(snookerGetCueBall(snooker),D_2_PI * (float)rand() / (float)RAND_MAX,speed);
}

void snookerTackle(PSnooker snooker, TFloat speed) {
	TVector2D	v;
	v.i = snooker->camera.xv - snooker->camera.xc;
	v.j = snooker->camera.yv - snooker->camera.yc;
	ballApplyMovement(snookerGetCueBall(snooker),vectorToAngle(&v),speed);
}

int snookerSetupOfficialBallColors(PSnooker snooker) {
	if ( snooker->balls != D_BALLS ) {
		return 0;
	}
	ballSetColor2D(snooker->ball[D_CUEBALL],1.0,1.0,1.0);
	ballSetColor2D(snooker->ball[1],1.0,0.0,0.0);
	ballSetColor2D(snooker->ball[2],1.0,0.0,0.0);
	ballSetColor2D(snooker->ball[3],1.0,0.0,0.0);
	ballSetColor2D(snooker->ball[4],1.0,0.0,0.0);
	ballSetColor2D(snooker->ball[5],1.0,0.0,0.0);
	ballSetColor2D(snooker->ball[6],1.0,0.0,0.0);
	ballSetColor2D(snooker->ball[7],1.0,0.0,0.0);
	ballSetColor2D(snooker->ball[8],1.0,0.0,0.0);
	ballSetColor2D(snooker->ball[9],1.0,0.0,0.0);
	ballSetColor2D(snooker->ball[10],1.0,0.0,0.0);
	ballSetColor2D(snooker->ball[11],1.7,0.0,0.0);
	ballSetColor2D(snooker->ball[12],1.0,0.0,0.0);
	ballSetColor2D(snooker->ball[12],1.0,0.0,0.0);
	ballSetColor2D(snooker->ball[13],1.0,0.0,0.0);
	ballSetColor2D(snooker->ball[14],1.0,0.0,0.0);
	ballSetColor2D(snooker->ball[15],1.0,0.0,0.0);
	ballSetColor2D(snooker->ball[16],0.0,0.0,0.0);
	ballSetColor2D(snooker->ball[17],1.0,1.0,0.0);
	ballSetColor2D(snooker->ball[18],0.0,1.0,0.0);
	ballSetColor2D(snooker->ball[19],0.0,0.0,1.0);
	ballSetColor2D(snooker->ball[20],1.0,0.0,1.0);
	ballSetColor2D(snooker->ball[21],0.4,0.24,0.19);
	ballSetColor3D(snooker->ball[D_CUEBALL],1.0,1.0,1.0);
	ballSetColor3D(snooker->ball[1],0.7,0.0,0.0);
	ballSetColor3D(snooker->ball[2],0.7,0.0,0.0);
	ballSetColor3D(snooker->ball[3],0.7,0.0,0.0);
	ballSetColor3D(snooker->ball[4],0.7,0.0,0.0);
	ballSetColor3D(snooker->ball[5],0.7,0.0,0.0);
	ballSetColor3D(snooker->ball[6],0.7,0.0,0.0);
	ballSetColor3D(snooker->ball[7],0.7,0.0,0.0);
	ballSetColor3D(snooker->ball[8],0.7,0.0,0.0);
	ballSetColor3D(snooker->ball[9],0.7,0.0,0.0);
	ballSetColor3D(snooker->ball[10],0.7,0.0,0.0);
	ballSetColor3D(snooker->ball[11],0.7,0.0,0.0);
	ballSetColor3D(snooker->ball[12],0.7,0.0,0.0);
	ballSetColor3D(snooker->ball[12],0.7,0.0,0.0);
	ballSetColor3D(snooker->ball[13],0.7,0.0,0.0);
	ballSetColor3D(snooker->ball[14],0.7,0.0,0.0);
	ballSetColor3D(snooker->ball[15],0.7,0.0,0.0);
	ballSetColor3D(snooker->ball[16],0.0,0.0,0.0);
	ballSetColor3D(snooker->ball[17],0.7,0.7,0.0);
	ballSetColor3D(snooker->ball[18],0.0,0.7,0.0);
	ballSetColor3D(snooker->ball[19],0.0,0.0,0.7);
	ballSetColor3D(snooker->ball[20],0.7,0.0,0.7);
	ballSetColor3D(snooker->ball[21],0.4,0.24,0.19);
	return 1;
}

void snookerSetCameraXC(PSnooker snooker, TFloat x) {
	snooker->camera.xc = x;
	snooker->camera.changed = 1;
}

void snookerSetCameraYC(PSnooker snooker, TFloat y) {
	snooker->camera.yc = y;
	snooker->camera.changed = 1;
}

void snookerSetCameraZC(PSnooker snooker, TFloat height) {
	snooker->camera.zc = ( height >= D_TABLE_FLOOR_HEIGHT + 0.1 ) ? height : D_TABLE_FLOOR_HEIGHT + 0.1;
	snooker->camera.changed = 1;
}

void snookerSetCameraXV(PSnooker snooker, TFloat x) {
	snooker->camera.xv = x;
	snooker->camera.changed = 1;
}

void snookerSetCameraYV(PSnooker snooker, TFloat y) {
	snooker->camera.yv = y;
	snooker->camera.changed = 1;
}

void snookerSetCameraZV(PSnooker snooker, TFloat z) {
	snooker->camera.zv = z;
	snooker->camera.changed = 1;
}

void snookerSetCameraD(PSnooker snooker, TFloat distance) {
	snooker->camera.d = fabs(distance);
	snooker->camera.changed = 1;
}

void snookerSetCameraAngle(PSnooker snooker, TFloat angle) {
	snooker->camera.xc = snooker->camera.xv - snooker->camera.d * cos(angle);
	snooker->camera.yc = snooker->camera.yv - snooker->camera.d * sin(angle);
}

void snookerAddCameraAngle(PSnooker snooker, TFloat angle) {
	TFloat		a;
	TVector2D	v;
	v.i = snooker->camera.xv - snooker->camera.xc;
	v.j = snooker->camera.yv - snooker->camera.yc;
	a = vectorToAngle(&v) + angle;
	snooker->camera.xc = snooker->camera.xv - snooker->camera.d * cos(a);
	snooker->camera.yc = snooker->camera.yv - snooker->camera.d * sin(a);
}

void snookerSetCameraMode(PSnooker snooker, int mode) {
	snooker->camera.mode = mode;
	snooker->camera.changed = 1;
}

void snookerAddCameraC(PSnooker snooker, TFloat x, TFloat y, TFloat z) {
	snooker->camera.xc += x;
	snooker->camera.yc += y;
	snooker->camera.zc += z;
}

void snookerCameraUpdate(PSnooker snooker) {
	TFloat	border = tableGetBorder(snooker->table);
	if ( snooker->camera.mode == D_CAMERA_MODE_CENTERED || snooker->camera.mode == D_CAMERA_MODE_WATCH) {
		snooker->camera.xv = border + ballGetX(snookerGetCueBall(snooker));
		snooker->camera.yv = border + ballGetY(snookerGetCueBall(snooker));
		snooker->camera.zv = D_TABLE_FLOOR_HEIGHT + ballGetRadius(snookerGetCueBall(snooker));
	}
	if ( !snooker->camera.changed ) {
		return;
	}
	TFloat		y, x, x1, x2, a, b, k, c, tx, ty, n;
	TVector2D	v;
	switch ( snooker->camera.mode ) {
		case D_CAMERA_MODE_CENTERED:
//			d = distancePointToPoint(vertex(snooker->camera.xc,snooker->camera.yc),
//										vertex(snooker->camera.xv,snooker->camera.yv));
//			if ( d != snooker->camera.d ) {
//				TVector2D	v;
//				v.i = snooker->camera.xv - snooker->camera.xc;
//				v.j = snooker->camera.yv - snooker->camera.yc;
//				vectorNormalize(&v);
//				snooker->camera.xc = snooker->camera.d * v.i + snooker->camera.xv;
//				snooker->camera.yc = snooker->camera.d * v.j + snooker->camera.yv;
//			}
			break;
		case D_CAMERA_MODE_FREE:
			// nothing.
			break;
		case D_CAMERA_MODE_WATCH:
			tx = tableGetTotalWidth(snooker->table) / 2.0;
			ty = tableGetTotalHeight(snooker->table) / 2.0;
			a = tx * 1.4;
			b = ty * 1.4;
			k = angularCoef(vertex(snooker->camera.xc,snooker->camera.yc),vertex(snooker->camera.xv,snooker->camera.yv));
			c = (border + ballGetY(snookerGetCueBall(snooker)) - ty) - k * (border + ballGetX(snookerGetCueBall(snooker)) - tx);
			equationSolve2(sqr(k) + sqr(b) / sqr(a),2 * k * c,sqr(c) - sqr(b),&x1,&x2);
			v.i = snooker->camera.xv - snooker->camera.xc;
			v.j = snooker->camera.yv - snooker->camera.yc;
			n = vectorToAngle(&v);
			if ( n < D_PI_2 ) {
				x = x2;
			} else if ( n < D_PI ) {
				x = x1;
			} else if ( n < D_3_PI_2 ) {
				x = x1;
			} else {
				x = x2;
			}
			y = k * x + c;
			snooker->camera.xc = border + x + tx;
			snooker->camera.yc = border + y + ty;
			break;
		case D_CAMERA_MODE_FAR:
			snooker->camera.xc = -1.8;
			snooker->camera.yc = -1.8;
			snooker->camera.zc = 1.8;
			snooker->camera.xv = tableGetTotalWidth(snooker->table);
			snooker->camera.yv = tableGetTotalHeight(snooker->table);
			snooker->camera.zv = D_TABLE_FLOOR_HEIGHT;
			break;
	}
	snooker->camera.changed = 0;
}

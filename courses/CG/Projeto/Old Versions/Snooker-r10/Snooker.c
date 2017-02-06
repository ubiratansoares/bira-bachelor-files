#include	"Snooker.h"

PSnooker snookerCreate() {
	PSnooker snooker = malloc(sizeof(TSnooker));
	snooker->paused = 1;
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
		ballSetColor(snooker->ball[i],rand() / (TFloat)RAND_MAX,rand() / (TFloat)RAND_MAX,rand() / (TFloat)RAND_MAX);
		ballSetXY(snooker->ball[i],(i + 1) * snooker->table->width / (balls + 1),snooker->table->height / 2.0);
		ballApplyMovement(snooker->ball[i],0.0,0.0);
	}
}

void snookerSetupWhiteBall(PSnooker snooker, TFloat radius, TFloat mass) {
	ballSetup(snookerGetWhiteBall(snooker),radius,mass);
	ballSetColor(snookerGetWhiteBall(snooker),1.0,1.0,1.0);
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

PBall snookerGetWhiteBall(PSnooker snooker) {
	return snooker->ball[0];
}

unsigned snookerGetBalls(PSnooker snooker) {
	return snooker->balls;
}

PBall snookerGetBall(PSnooker snooker, unsigned ball) {
	return snooker->ball[ball];
}

void snookerPaint(PSnooker snooker) {
	unsigned	i;
	TFloat		border = tableGetBorder(snooker->table);
	tablePaint(snooker->table);
	for ( i = 0; i < snooker->balls; i++ ) {
		if ( !ballGetFlag(snooker->ball[i],BALL_DEAD) ) {
			ballPaint(snooker->ball[i],border);
		}
	}
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

	if ( timeElapsed > M_SNOOKER_THINK_MAX_TIME_SLICE ) {
		#ifdef DEBUG
			puts("- Splitting timeElapsed!");
		#endif
		timeElapsed -= M_SNOOKER_THINK_MAX_TIME_SLICE;
		snookerThink(snooker,timeElapsed);
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
		if ( snooker->ball[i] != snookerGetWhiteBall(snooker) && !ballGetFlag(snooker->ball[i],BALL_DEAD) ) {
			return 0;
		}
	}
	return 1;
}

void snookerTackleRandomBall(PSnooker snooker, TFloat speed) {
	unsigned	iball = 0;
	PBall		wball = snookerGetWhiteBall(snooker), ball;
	do {
		iball = rand() % snooker->balls;
		ball = snooker->ball[iball];
	} while ( ball == wball && ballGetFlag(ball,BALL_DEAD) );
	TFloat	angle = atan(angularCoef(vertex(wball->x,wball->y),vertex(ball->x,ball->y)));
	if ( ball->x < wball->x ) {
		angle += MPI;
	}
	if ( angle < 0 ) {
		angle += M2_PI;
	} else if ( angle > M2_PI ) {
		angle -= M2_PI;
	}
	ballApplyMovement(wball,angle,speed);
}

void snookerTackleClosestBall(PSnooker snooker, TFloat speed) {
	unsigned	i, iball = 0;
	TFloat		d = 1000.0, t;
	PBall		wball = snookerGetWhiteBall(snooker), ball;
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
		angle += M_PI;
	}
	if ( angle < 0 ) {
		angle += M2_PI;
	} else if ( angle > M2_PI ) {
		angle -= M2_PI;
	}
	ballApplyMovement(wball,angle,speed);
}

void snookerTackleFartestBall(PSnooker snooker, TFloat speed) {
	unsigned	i, iball = 0;
	TFloat		d = 0, t;
	PBall		wball = snookerGetWhiteBall(snooker), ball;
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
		angle += MPI;
	}
	if ( angle < 0 ) {
		angle += M2_PI;
	} else if ( angle > M2_PI ) {
		angle -= M2_PI;
	}
	ballApplyMovement(wball,angle,speed);
}

void snookerTackleRandom(PSnooker snooker, TFloat speed) {
	ballApplyMovement(snookerGetWhiteBall(snooker),M2_PI * (float)rand() / (float)RAND_MAX,speed);
}

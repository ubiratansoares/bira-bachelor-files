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
	for ( i = 0; i < snooker->balls; i++ )
		ballPaint(snooker->ball[i],border);
}

void snookerStepBalls(PSnooker snooker, TFloat time) {
	#ifdef DEBUG
		puts("\nsnookerStepBalls(snooker,time)");
	#endif
	int		i;
	TFloat	m = tableGetAtrCoef(snooker->table);
	for ( i = 0; i < snooker->balls; i++ ) {
		#ifdef DEBUG
			printf("  %d ",i);
		#endif
		ballStep(snooker->ball[i],m,time);
	}
}

void snookerThink(PSnooker snooker, TFloat timeElapsed) {

	if ( timeElapsed > SNOOKER_THINK_MAX_TIME_SLICE ) {
		#ifdef DEBUG
			printf("\n> snookerThink(snooker,%.6f)\n",timeElapsed);
		#endif
		do {
			#ifdef DEBUG
				puts("- Splitting timeElapsed!");
			#endif
			snookerThink(snooker,SNOOKER_THINK_MAX_TIME_SLICE);
			timeElapsed -= SNOOKER_THINK_MAX_TIME_SLICE;
		} while ( timeElapsed > 0.0 );
		#ifdef DEBUG
			puts("- Split Done!");
		#endif
		return;
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
		int			hitWithTable = 1, hitTableType = 0;
		unsigned	hitBall1 = 0, hitBall2 = 0;

		// check if any ball collided with the table.
		for ( i = 0; i < balls; i++ ) {
			if ( ballSpeed(ball[i]) != 0 ) {
				// a collision happened? How was this collision?
				int	tempHitTableType = tableCollided(table,ball[i]);
				if ( tempHitTableType ) {
					#ifdef DEBUG
						puts("  - Collision Detected");
						printf("    ball=%u hitTableType=%u X=%.6f Y=%.6f R=%.6f\n",i,tempHitTableType,ball[i]->x,ball[i]->y,ball[i]->radius);
//						getch();
					#endif
					// get the actual time of the collision.
					TFloat	timeTemp = tableCollisionTime(table,ball[i],tempHitTableType);
					if ( timeTemp > time ) {
						time = timeTemp;
						hitTableType = tempHitTableType;
						hitWithTable = 1;
						hitBall1 = i;
						hit = 1;
						#ifdef DEBUG
							puts("  - Previous Collision Detected: Table");
							printf("    time=%.6f ball=%u hitTableType=%u X=%.6f Y=%.6f R=%.6f\n",time,hitBall1,hitTableType,ball[i]->x,ball[i]->y,ball[i]->radius);
						#endif
					}
				}
			}
		}

		// check if collided with another ball.
		for ( i = 0; i < balls - 1; i++ ) {
			for ( j = i + 1; j < balls; j++ ) {
				// a collision happened.
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
				#ifdef DEBUG
					puts("- Table Hit");
					printf("  B=%u X=%.6f Y=%.6f A=%.3f V=%.3f\n",hitBall1,snooker->ball[hitBall1]->x,snooker->ball[hitBall1]->y,snooker->ball[hitBall1]->angle,snooker->ball[hitBall1]->speed);
				#endif
				tableCollision(snooker->table,snooker->ball[hitBall1],hitTableType);
				#ifdef DEBUG
					printf("  A=%.3f V=%.3f\n",snooker->ball[hitBall1]->angle,snooker->ball[hitBall1]->speed);
				#endif
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

	#ifdef ERROR_CHECK
		for ( i = 0; i < balls; i++ ) {
			if ( (ball[i]->x + ball[i]->radius > table->width) || (ball[i]->x - ball[i]->radius < 0.0) ||
			(ball[i]->y + ball[i]->radius > table->height) || (ball[i]->y - ball[i]->radius < 0.0) ) {
				ballSetFlag(ball[i],BALL_DRAW_ERROR_SIGNAL);
				snookerPause(snooker,1);
				puts("\n> Error: Ball Out Of Table Bounds");
				#ifdef DEBUG
					puts("  Compilation=DEBUG");
				#endif
				puts("- Flags");
				printf("  hit=%u hitWithTable=%u hitTableType=%u hitBall1=%u hitBall2=%u\n",hit,hitWithTable,hitTableType,hitBall1,hitBall2);
				puts("- Table");
				printf("  W=%.6f H=%.6f\n",table->width,table->height);
				puts("- Ball");
				printf("  B=%u X=%.6f Y=%.6f R=%.6f M=%.3f A=%.3f V=%.3f F=%u\n",i,ball[i]->x,ball[i]->y,ball[i]->radius,ball[i]->mass,ball[i]->angle,ball[i]->speed,ball[i]->flags);
				printf("  Bottom=%.6f Left=%.6f Top=%.6f Right=%.6f\n",ball[i]->y - ball[i]->radius,ball[i]->x - ball[i]->radius,ball[i]->y + ball[i]->radius,ball[i]->x + ball[i]->radius);
				// abort the execution.
				return;
			}
		}
	#endif

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

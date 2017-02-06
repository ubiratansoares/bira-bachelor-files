#ifndef SNOOKERDEFS_H_INCLUDED
#define SNOOKERDEFS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

// Comment this line to remove any debug markees.
//#define	DEBUG

// Comment this to disable error checking.
//#define	ERROR_CHECK

// Uncomment to disable the initialization of the random seed (good for debugging equations).
//#define	DISABLE_RANDSEED

// Uncomment this line to use double instead of float when calculating.
#define	USE_DOUBLE

#ifdef	USE_DOUBLE
	#define	TFloat						double
	#define	PFloat						double*
#else
	#define	TFloat						float
	#define	PFloat						float*
#endif

#define D_PI_4							0.78539816339744830962
#define D_PI_2							1.57079632679489661923
#define	D_3_PI_4						2.35619449019234492885
#define D_PI							3.14159265358979323846
#define	D_5_PI_4						3.92699081698724154808
#define D_3_PI_2						4.71238898038468985769
#define	D_7_PI_4						5.49778714378213816731
#define D_2_PI							6.28318530717958647693
#define	D_COS_PI_4						0.70710678118654752440
#define	D_SIN_PI_4						0.70710678118654752440
#define	D_COS_3_PI_4					-0.70710678118654752440
#define	D_SIN_3_PI_4					0.70710678118654752440
#define	D_COS_5_PI_4					-0.70710678118654752440
#define	D_SIN_5_PI_4					-0.70710678118654752440
#define	D_COS_7_PI_4					0.70710678118654752440
#define	D_SIN_7_PI_4					-0.70710678118654752440

#define D_SQRT_2						1.41421356237309504880
#define	D_SQRT_3						1.73205080756887729353
#define	D_SIN_PI_4_DIV_SQRT_3_2			0.57735026918962576451

#define D_5_4							1.25

#define	D_HIT_TOP						0x01
#define	D_HIT_BOTTOM					0x02
#define	D_HIT_LEFT						0x04
#define	D_HIT_RIGHT						0x08
#define D_HIT_V							0x03
#define D_HIT_H							0x0C

#define	D_G								9.80665

#define	D_SNOOKER_THINK_MAX_TIME_SLICE	0.0005

#define	D_TABLE_BAULKLINE_DISTANCE		0.737
#define	D_TABLE_BAULK_RADIUS			0.292
#define D_TABLE_ASPECT					2.007311586051743532	// D_TABLE_ASPECT = D_TABLE_WIDTH / D_TABLE_HEIGHT
#define	D_TABLE_WIDTH					3.569
#define	D_TABLE_HEIGHT					1.778
#define	D_TABLE_BORDER					0.2
#define	D_TABLE_FLOOR_HEIGHT			0.851
#define	D_TABLE_POCKET_DIAMETER			0.07
#define	D_TABLE_POCKET_MOUTH			1.2
#define	D_TABLE_HIT_SPEED_RATIO			0.85

#define	D_BALL_ELASTIC_COEFFICIENT		0.85
#define	D_BALLS							22						// 21 + 1 cue ball.
#define	D_BALL_RADIUS					0.02625
#define	D_BALL_MASS						1.0
#define	D_CUEBALL_RADIUS				0.02625
#define	D_CUEBALL_MASS					1.0

#define	D_CAMERA_DISTANCE				0.4
#define	D_CAMERA_HEIGHT					1.0
#define	D_CAMERA_MODE_CENTERED			1
#define	D_CAMERA_MODE_FREE				2
#define	D_CAMERA_MODE_WATCH				3
#define	D_CAMERA_MODE_FAR				4

#define	D_ARC_QUALITY					10
#define	D_CIRCLE_QUALITY				30
#define	D_SPHERE_QUALITY				20

#define	D_LIGHT_BALL_SHININESS			32.0
#define	D_LIGHT_TABLE_SHININESS			0.0

#define	D_PLANE_DIFF					0.00001

#endif // SNOOKERDEFS_H_INCLUDED

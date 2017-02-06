#ifndef SNOOKERDEFS_H_INCLUDED
#define SNOOKERDEFS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
#else
	#define	TFloat						float
#endif

#define M_5_4							1.25


#define MPI_4							0.78539816339744830962
#define MPI_2							1.57079632679489661923
#define MPI							3.14159265358979323846
#define M3_PI_2						4.71238898038468985769
#define M2_PI							6.28318530717958647693
#define	MCOS_PI_4						0.70710678118654752440
#define	MSIN_PI_4						0.70710678118654752440

#define MSQRT_2						1.41421356237309504880


#define	M_HIT_TOP						0x01
#define	M_HIT_BOTTOM					0x02
#define	M_HIT_LEFT						0x04
#define	M_HIT_RIGHT						0x08
#define M_HIT_V							0x03
#define M_HIT_H							0x0C

#define	M_G								9.80665

#define	M_TABLE_HIT_SPEED_RATIO			0.85

#define	M_BALL_ELASTIC_COEFFICIENT		0.85

#define	M_SNOOKER_THINK_MAX_TIME_SLICE	0.0005

#define	M_POCKET_MOUTH					1.2

#endif // SNOOKERDEFS_H_INCLUDED

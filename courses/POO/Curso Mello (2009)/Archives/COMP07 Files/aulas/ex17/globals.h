#ifndef _GLOBALS_H_
#define _GLOBALS_H_

typedef union {
	char opt;
	float data;
} DATA;

typedef struct {
	DATA value;
	int flag:1; // 0 - char; 1 - float
} DATA_TYPE;

#endif

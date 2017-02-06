#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>

typedef	struct {
	unsigned char	r, g, b;
} TRGB, *PRGB;

typedef struct {
	unsigned	glID, width, height;
	char*		fileName;
	PRGB		pixels;
} TTexture, *PTexture;

PTexture	textureCreate(char* fileName);
void		textureRelease(PTexture tex);
void		textureDestroy(PTexture *tex);

#endif // TEXTURE_H_INCLUDED

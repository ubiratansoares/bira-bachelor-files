#include	"Texture.h"

void* textureRawLoad(char* fileName, unsigned* width, unsigned* height) {
	char	parFileName[256];
	FILE	*f, *p;
	void	*data;
	parFileName[0] = 0;
	f = fopen(fileName,"r");
	if ( !f ) {
		printf("textureRawLoad(%s): file not found.\n",fileName);
	}
	p = fopen(strcat(strcpy(parFileName,fileName),".dim"),"r");
	if ( !p ) {
		printf("textureRawLoad(%s): file not found.\n",parFileName);
	}
	fscanf(p,"%u %u",width,height);
	data = malloc(3 * (*width) * (*height));
	fread(data,3 * (*width) * (*height),1,f);
	fclose(f);
	fclose(p);
	return data;
}

PTexture textureCreate(char* fileName) {
	PTexture	tex;
	tex = (PTexture)malloc(sizeof(TTexture));
	tex->pixels = (PRGB)textureRawLoad(fileName,&(tex->width),&(tex->height));
	tex->fileName = malloc(strlen(fileName) + 1);
	tex->fileName[0] = 0;
	strcpy(tex->fileName,fileName);
	tex->glID = 0;
	return tex;
}

void textureRelease(PTexture tex) {
	if ( tex->pixels ) {
		free(tex->pixels);
	}
}

void textureDestroy(PTexture *t) {
	PTexture	tex = *t;
	if ( tex->fileName ) {
		free(tex->fileName);
	}
	textureRelease(tex);
	free(tex);
	*t = 0;
}

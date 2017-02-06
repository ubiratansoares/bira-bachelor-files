#include <stdlib.h>
#include <math.h>
#include "Table.h"

PTable tableCreate(TFloat totalWidth, TFloat totalHeight) {
	PTable table = malloc(sizeof(TTable));
	table->totalWidth = totalWidth;
	table->totalHeight = totalHeight;
	table->pocket = (PVertex)malloc(sizeof(TVertex) * TABLE_POCKETS);
	return table;
}

void tableDestroy(PTable *table) {
	if ( table && *table ) {
		free((*table)->pocket);
		free(*table);
		*table = 0;
	}
}

void tableSetup(PTable t, TFloat width, TFloat height, TFloat pocketSize, TFloat atrCoef) {
	t->width = width;
	t->height = height;
	t->pocketSize = pocketSize;
	t->atrCoef = atrCoef;
	t->pockets = 0x22222;

	TFloat	slice = pocketSize / D_SQRT_2;
	TFloat	pocketRadius = pocketSize / 2.0;
	TFloat	recoil = slice;
	TFloat	border = tableGetBorder(t);

	t->pocket[0].x = border - D_COS_PI_4 * slice;				t->pocket[0].y = border - D_SIN_PI_4 * slice;
	t->pocket[1].x = border - D_COS_PI_4 * slice;				t->pocket[1].y = t->height + border + D_SIN_PI_4 * slice;
	t->pocket[2].x = t->totalWidth / 2.0;						t->pocket[2].y = t->height + border + pocketRadius;
	t->pocket[3].x = t->width + border + D_COS_PI_4 * slice;	t->pocket[3].y = t->height + border + D_SIN_PI_4 * slice;
	t->pocket[4].x = t->width + border + D_COS_PI_4 * slice;	t->pocket[4].y = border - D_SIN_PI_4 * slice;
	t->pocket[5].x = t->totalWidth / 2.0;						t->pocket[5].y = border - pocketRadius;

	t->bound[0] = lineCreate(vertex(0.0 + D_TABLE_POCKET_MOUTH * slice,0.0),	vertex(t->pocket[0].x + pocketRadius * cos(D_7_PI_4) - border,t->pocket[0].y + pocketRadius * sin(D_7_PI_4) - border));
	t->bound[1] = lineCreate(t->bound[0]->p2,									vertex(t->pocket[0].x + pocketRadius * cos(D_3_PI_4) - border,t->pocket[0].y + pocketRadius * sin(D_3_PI_4) - border));
	t->bound[2] = lineCreate(t->bound[1]->p2,									vertex(0.0,0.0 + D_TABLE_POCKET_MOUTH * slice));

	t->bound[3] = lineCreate(t->bound[2]->p2,									vertex(0.0,t->height + 0.0 - D_TABLE_POCKET_MOUTH * slice));

	t->bound[4] = lineCreate(t->bound[3]->p2,									vertex(t->pocket[1].x + pocketRadius * cos(D_5_PI_4) - border,t->pocket[1].y + pocketRadius * sin(D_5_PI_4) - border));
	t->bound[5] = lineCreate(t->bound[4]->p2,									vertex(t->pocket[1].x + pocketRadius * cos(D_PI_4) - border,t->pocket[1].y + pocketRadius * sin(D_PI_4) - border));
	t->bound[6] = lineCreate(t->bound[5]->p2,									vertex(0.0 + D_TABLE_POCKET_MOUTH * slice,t->height + 0.0));

	t->bound[7] = lineCreate(t->bound[6]->p2,									vertex(t->width / 2.0 + 0.0 - D_SQRT_2 * 0.5 * D_TABLE_POCKET_MOUTH * slice,t->height + 0.0));

	t->bound[8] = lineCreate(t->bound[7]->p2,									vertex(t->width / 2.0 + 0.0 - D_SQRT_2 * 0.5 * slice,t->height + 0.0 + recoil));
	t->bound[9] = lineCreate(t->bound[8]->p2,									vertex(t->width / 2.0 + 0.0 + D_SQRT_2 * 0.5 * slice,t->height + 0.0 + recoil));
	t->bound[10] = lineCreate(t->bound[9]->p2,									vertex(t->width / 2.0 + 0.0 + D_SQRT_2 * 0.5 * D_TABLE_POCKET_MOUTH * slice,t->height + 0.0));

	t->bound[11] = lineCreate(t->bound[10]->p2,									vertex(t->width + 0.0 - D_TABLE_POCKET_MOUTH * slice,t->height + 0.0));

	t->bound[12] = lineCreate(t->bound[11]->p2,									vertex(t->pocket[3].x + pocketRadius * cos(D_3_PI_4) - border,t->pocket[3].y + pocketRadius * sin(D_3_PI_4) - border));
	t->bound[13] = lineCreate(t->bound[12]->p2,									vertex(t->pocket[3].x + pocketRadius * cos(D_7_PI_4) - border,t->pocket[3].y + pocketRadius * sin(D_7_PI_4) - border));
	t->bound[14] = lineCreate(t->bound[13]->p2,									vertex(t->width + 0.0,t->height + 0.0 - D_TABLE_POCKET_MOUTH * slice));

	t->bound[15] = lineCreate(t->bound[14]->p2,									vertex(t->width + 0.0,0.0 + D_TABLE_POCKET_MOUTH * slice));

	t->bound[16] = lineCreate(t->bound[15]->p2,									vertex(t->pocket[4].x + pocketRadius * cos(D_PI_4) - border,t->pocket[4].y + pocketRadius * sin(D_PI_4) - border));
	t->bound[17] = lineCreate(t->bound[16]->p2,									vertex(t->pocket[4].x + pocketRadius * cos(D_5_PI_4) - border,t->pocket[4].y + pocketRadius * sin(D_5_PI_4) - border));
	t->bound[18] = lineCreate(t->bound[17]->p2,									vertex(t->width + 0.0 - D_TABLE_POCKET_MOUTH * slice,0.0));

	t->bound[19] = lineCreate(t->bound[18]->p2,									vertex(t->width / 2.0 + 0.0 + D_SQRT_2 * 0.5 * D_TABLE_POCKET_MOUTH * slice,0.0));

	t->bound[20] = lineCreate(t->bound[19]->p2,									vertex(t->width / 2.0 + 0.0 + D_SQRT_2 * 0.5 * slice,0.0 - recoil));
	t->bound[21] = lineCreate(t->bound[20]->p2,									vertex(t->width / 2.0 + 0.0 - D_SQRT_2 * 0.5 * slice,0.0 - recoil));
	t->bound[22] = lineCreate(t->bound[21]->p2,									vertex(t->width / 2.0 + 0.0 - D_SQRT_2 * 0.5 * D_TABLE_POCKET_MOUTH * slice,0.0));

	t->bound[23] = lineCreate(t->bound[22]->p2,									t->bound[0]->p1);

	char	fileName[1024];

	fileName[0] = 0;
	t->wood = textureCreate(strcat(strcpy(fileName,currentDir),"wood.raw"));
	glGenTextures(1,&(t->wood->glID));
	glBindTexture(GL_TEXTURE_2D,t->wood->glID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,t->wood->width,t->wood->height,0,GL_RGB,GL_UNSIGNED_BYTE,t->wood->pixels);
	textureRelease(t->wood);

	fileName[0] = 0;
	t->field = textureCreate(strcat(strcpy(fileName,currentDir),"field.raw"));
	glGenTextures(1,&(t->field->glID));
	glBindTexture(GL_TEXTURE_2D,t->field->glID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,t->field->width,t->field->height,0,GL_RGB,GL_UNSIGNED_BYTE,t->field->pixels);
	textureRelease(t->field);

	fileName[0] = 0;
	t->steel = textureCreate(strcat(strcpy(fileName,currentDir),"steel.raw"));
	glGenTextures(1,&(t->steel->glID));
	glBindTexture(GL_TEXTURE_2D,t->steel->glID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,t->steel->width,t->steel->height,0,GL_RGB,GL_UNSIGNED_BYTE,t->steel->pixels);
	textureRelease(t->steel);

	//free(currentDir);

	#ifdef DEBUG
		glBegin(GL_LINES);
		glColor3f(1,0,0);
			int	i = 0;
			for ( i = 0; i < TABLE_BOUNDS; i++ ) {
				glVertex2f(t->bound[i]->p1.x + border,t->bound[i]->p1.y + border);
				glVertex2f(t->bound[i]->p2.x + border,t->bound[i]->p2.y + border);
			}
		glEnd();
	#endif
}

TFloat tableGetBorderHeight(PTable t, TFloat ballRadius) {
	return D_TABLE_FLOOR_HEIGHT + 1.5 * ballRadius;
}

void tablePaintCornerPocketBorder3D(PTable t, TFloat borderHeight) {
	TFloat	pocketRadius = t->pocketSize / 2.0;
	TFloat	x, y, slice, angle, fX, fY;
	fX = 1.0 / (tableGetBorder(t) + t->bound[1]->p1.x);
	fY = 1.0 / (tableGetBorder(t) + t->bound[2]->p1.y);
	glBegin(GL_TRIANGLE_STRIP);
		slice = (D_7_PI_4 - D_PI) / D_ARC_QUALITY / 2.0;			// to make the quality equal in all cases.
		for ( angle = D_PI; angle < D_7_PI_4; angle += slice ) {
			x = t->pocket[0].x + pocketRadius * cos(angle);
			y = t->pocket[0].y + pocketRadius * sin(angle);
			glTexCoord2f(x * fX,y * fY);
			glVertex3f(x,y,borderHeight);
			glTexCoord2f(x * fX,0.0);
			glVertex3f(x,0.0,borderHeight);
		}
		x = t->pocket[0].x + pocketRadius * cos(D_7_PI_4);
		y = t->pocket[0].y + pocketRadius * sin(D_7_PI_4);
		glTexCoord2f(x * fX,y * fY);
		glVertex3f(x,y,borderHeight);
		glTexCoord2f(x * fX,0.0);
		glVertex3f(x,0.0,borderHeight);
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
		slice = (D_PI - D_3_PI_4) / D_ARC_QUALITY;
		for ( angle = D_3_PI_4; angle < D_PI; angle += slice ) {
			x = t->pocket[0].x + pocketRadius * cos(angle);
			y = t->pocket[0].y + pocketRadius * sin(angle);
			glTexCoord2f(x * fX,y * fY);
			glVertex3f(x,y,borderHeight);
			glTexCoord2f(0.0,y * fY);
			glVertex3f(0.0,y,borderHeight);
		}
		x = t->pocket[0].x + pocketRadius * cos(D_PI);
		y = t->pocket[0].y + pocketRadius * sin(D_PI);
		glTexCoord2f(x * fX,y * fY);
		glVertex3f(x,y,borderHeight);
		glTexCoord2f(0.0,y * fY);
		glVertex3f(0.0,y,borderHeight);
	glEnd();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,									t->pocket[0].y * fY);
		glVertex3f(0.0,										t->pocket[0].y,	borderHeight);
		glTexCoord2f((t->pocket[0].x - pocketRadius) * fX,	t->pocket[0].y * fY);
		glVertex3f(t->pocket[0].x - pocketRadius,			t->pocket[0].y,	borderHeight);
		glTexCoord2f((t->pocket[0].x - pocketRadius) * fX,	0.0);
		glVertex3f(t->pocket[0].x - pocketRadius,			0.0,			borderHeight);
		glTexCoord2f(0.0,									0.0);
		glVertex3f(0.0,										0.0,			borderHeight);
	glEnd();
}

void tablePaintMiddlePocketBorder3D(PTable t, TFloat borderHeight) {
	TFloat	pocketRadius = t->pocketSize / 2.0;
	TFloat	x, y, slice, angle, fX, fY;
	fX = 1.0 / (tableGetBorder(t) + t->bound[1]->p1.x);
	fY = 1.0 / (tableGetBorder(t) + t->bound[2]->p1.y);
	glBegin(GL_TRIANGLE_STRIP);
		slice = D_PI / D_ARC_QUALITY;
		for ( angle = 0.0; angle < D_PI; angle += slice ) {
			x = t->pocket[2].x + pocketRadius * cos(angle);
			y = t->pocket[2].y + pocketRadius * sin(angle);
			glTexCoord2f(x * fX,y * fY);
			glVertex3f(x,y,borderHeight);
			glTexCoord2f(x * fX,t->totalHeight * fY);
			glVertex3f(x,t->totalHeight,borderHeight);
		}
		x = t->pocket[2].x + pocketRadius * cos(D_PI);
		y = t->pocket[2].y + pocketRadius * sin(D_PI);
		glTexCoord2f(x * fX,y * fY);
		glVertex3f(x,y,borderHeight);
		glTexCoord2f(x * fX,t->totalHeight * fY);
		glVertex3f(x,t->totalHeight,borderHeight);
	glEnd();
}

void tablePaint3D(PTable t, TFloat ballRadius) {

	TFloat	border = tableGetBorder(t);
	TFloat	borderHeight = tableGetBorderHeight(t,ballRadius);
	TFloat	pocketRadius = t->pocketSize / 2.0;
	GLfloat	zero[4], woodHi[4], woodLow[4], greenHi[4], greenLow[4], greyHi[4], white[4];

	p4ToGLfv(0.00,0.00,0.00,1.00,zero);
	p4ToGLfv(0.80,0.50,0.37,1.00,woodHi);
	p4ToGLfv(0.60,0.40,0.30,1.00,woodLow);
	p4ToGLfv(0.00,0.90,0.00,1.00,greenHi);
	p4ToGLfv(0.00,0.40,0.00,1.00,greenLow);
	p4ToGLfv(0.70,0.70,0.70,1.00,greyHi);
	p4ToGLfv(1.00,1.00,1.00,1.00,white);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);

	// draw the table foots.
	glMaterialfv(GL_FRONT,GL_AMBIENT,white);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,zero);
	glMaterialfv(GL_FRONT,GL_SPECULAR,zero);
	glMaterialf (GL_FRONT,GL_SHININESS,D_LIGHT_TABLE_SHININESS);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,t->wood->glID);
	glPushMatrix();
		glScalef(2 * border,2 * border,D_TABLE_FLOOR_HEIGHT - border);
		glTranslatef(0.5,0.5,0.5);
		drawTexturedCube(1.0);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(t->totalWidth - 2 * border,0.0,0.0);
		glScalef(2 * border,2 * border,D_TABLE_FLOOR_HEIGHT - border);
		glTranslatef(0.5,0.5,0.5);
		drawTexturedCube(1.0);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(t->totalWidth - 2 * border,t->totalHeight - 2 * border,0.0);
		glScalef(2 * border,2 * border,D_TABLE_FLOOR_HEIGHT - border);
		glTranslatef(0.5,0.5,0.5);
		drawTexturedCube(1.0);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0,t->totalHeight - 2 * border,0.0);
		glScalef(2 * border,2 * border,D_TABLE_FLOOR_HEIGHT - border);
		glTranslatef(0.5,0.5,0.5);
		drawTexturedCube(1.0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	// draw the table base.
	glMaterialfv(GL_FRONT,GL_AMBIENT,woodHi);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,zero);
	glMaterialfv(GL_FRONT,GL_SPECULAR,zero);
	glMaterialf (GL_FRONT,GL_SHININESS,D_LIGHT_TABLE_SHININESS);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,t->wood->glID);
	glPushMatrix();
		glTranslatef(border / 10.0,border / 10.0,D_TABLE_FLOOR_HEIGHT - border - D_TABLE_FLOOR_HEIGHT / 4.0);
		glScalef(t->totalWidth - border / 5.0,t->totalHeight - border / 5.0,D_TABLE_FLOOR_HEIGHT / 4.0);
		glTranslatef(0.5,0.5,0.5);
		drawTexturedCube(1.0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	// draw the external border.
	glMaterialfv(GL_FRONT,GL_AMBIENT,woodHi);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,zero);
	glMaterialfv(GL_FRONT,GL_SPECULAR,zero);
	glMaterialf (GL_FRONT,GL_SHININESS,D_LIGHT_TABLE_SHININESS);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,t->wood->glID);
	glBegin(GL_QUADS);
		glNormal3f(0.0,-1.0,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(0,0,D_TABLE_FLOOR_HEIGHT - border);
		glTexCoord2f(1.0,0.0);
		glVertex3f(0,0,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(t->totalWidth,0,borderHeight);
		glTexCoord2f(0.0,1.0);
		glVertex3f(t->totalWidth,0,D_TABLE_FLOOR_HEIGHT - border);

		glNormal3f(1.0,0.0,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(t->totalWidth,0,D_TABLE_FLOOR_HEIGHT - border);
		glTexCoord2f(0.0,1.0);
		glVertex3f(t->totalWidth,0,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(t->totalWidth,t->totalHeight,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(t->totalWidth,t->totalHeight,D_TABLE_FLOOR_HEIGHT - border);

		glNormal3f(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(t->totalWidth,t->totalHeight,D_TABLE_FLOOR_HEIGHT - border);
		glTexCoord2f(1.0,0.0);
		glVertex3f(0,t->totalHeight,D_TABLE_FLOOR_HEIGHT - border);
		glTexCoord2f(1.0,1.0);
		glVertex3f(0,t->totalHeight,borderHeight);
		glTexCoord2f(0.0,1.0);
		glVertex3f(t->totalWidth,t->totalHeight,borderHeight);

		glNormal3f(-1.0,0.0,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(0,t->totalHeight,D_TABLE_FLOOR_HEIGHT - border);
		glTexCoord2f(0.0,1.0);
		glVertex3f(0,0,D_TABLE_FLOOR_HEIGHT - border);
		glTexCoord2f(1.0,1.0);
		glVertex3f(0,0,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(0,t->totalHeight,borderHeight);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// pocket cilinders.
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT,GL_AMBIENT,white);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,zero);
	glMaterialfv(GL_FRONT,GL_SPECULAR,white);
	glMaterialf (GL_FRONT,GL_SHININESS,100);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,t->steel->glID);
	glPushMatrix();
		glTranslatef(t->pocket[0].x,t->pocket[0].y,D_TABLE_FLOOR_HEIGHT + 0.75 * ballRadius);
		glScalef(2 * pocketRadius,2 * pocketRadius,1.5 * ballRadius);
		glRotatef(135,0.0,0.0,1.0);
		drawTexturedCilinderSection(M_PI,20,1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(t->pocket[1].x,t->pocket[1].y,D_TABLE_FLOOR_HEIGHT + 0.75 * ballRadius);
		glScalef(2 * pocketRadius,2 * pocketRadius,1.5 * ballRadius);
		glRotatef(45,0.0,0.0,1.0);
		drawTexturedCilinderSection(M_PI,20,1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(t->pocket[2].x,t->pocket[2].y,D_TABLE_FLOOR_HEIGHT + 0.75 * ballRadius);
		glScalef(2 * pocketRadius,2 * pocketRadius,1.5 * ballRadius);
		drawTexturedCilinderSection(M_PI,20,1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(t->pocket[3].x,t->pocket[3].y,D_TABLE_FLOOR_HEIGHT + 0.75 * ballRadius);
		glScalef(2 * pocketRadius,2 * pocketRadius,1.5 * ballRadius);
		glRotatef(315,0.0,0.0,1.0);
		drawTexturedCilinderSection(M_PI,20,1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(t->pocket[4].x,t->pocket[4].y,D_TABLE_FLOOR_HEIGHT + 0.75 * ballRadius);
		glScalef(2 * pocketRadius,2 * pocketRadius,1.5 * ballRadius);
		glRotatef(225,0.0,0.0,1.0);
		drawTexturedCilinderSection(M_PI,20,1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(t->pocket[5].x,t->pocket[5].y,D_TABLE_FLOOR_HEIGHT + 0.75 * ballRadius);
		glScalef(2 * pocketRadius,2 * pocketRadius,1.5 * ballRadius);
		glRotatef(180,0.0,0.0,1.0);
		drawTexturedCilinderSection(M_PI,20,1);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	// table internal border.
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT,GL_AMBIENT,greenLow);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,greenLow);
	glMaterialfv(GL_FRONT,GL_SPECULAR,white);
	glMaterialf (GL_FRONT,GL_SHININESS,D_LIGHT_TABLE_SHININESS);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,t->field->glID);
	glNormal3f(0.0,0.0,1.0);
	glBegin(GL_QUADS);
//		glNormal3f(D_SIN_3_PI_4,D_COS_3_PI_4,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[0]->p1.x,border + t->bound[0]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[1]->p1.x,border + t->bound[1]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[1]->p1.x,border + t->bound[1]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[0]->p1.x,border + t->bound[0]->p1.y,borderHeight);

//		glNormal3f(D_SIN_7_PI_4,D_COS_7_PI_4,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[2]->p1.x,border + t->bound[2]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[2]->p1.x,border + t->bound[2]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[3]->p1.x,border + t->bound[3]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[3]->p1.x,border + t->bound[3]->p1.y,D_TABLE_FLOOR_HEIGHT);

//		glNormal3f(1.0,0.0,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[3]->p1.x,border + t->bound[3]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[3]->p1.x,border + t->bound[3]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[4]->p1.x,border + t->bound[4]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[4]->p1.x,border + t->bound[4]->p1.y,D_TABLE_FLOOR_HEIGHT);

//		glNormal3f(D_SIN_PI_4,D_COS_PI_4,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[4]->p1.x,border + t->bound[4]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[4]->p1.x,border + t->bound[4]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[5]->p1.x,border + t->bound[5]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[5]->p1.x,border + t->bound[5]->p1.y,D_TABLE_FLOOR_HEIGHT);

//		glNormal3f(D_SIN_5_PI_4,D_COS_5_PI_4,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[6]->p1.x,border + t->bound[6]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[6]->p1.x,border + t->bound[6]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[7]->p1.x,border + t->bound[7]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[7]->p1.x,border + t->bound[7]->p1.y,D_TABLE_FLOOR_HEIGHT);

//		glNormal3f(0.0,-1.0,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[7]->p1.x,border + t->bound[7]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[7]->p1.x,border + t->bound[7]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[8]->p1.x,border + t->bound[8]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[8]->p1.x,border + t->bound[8]->p1.y,D_TABLE_FLOOR_HEIGHT);

//		glNormal3f(D_SIN_7_PI_4,D_COS_7_PI_4,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[8]->p1.x,border + t->bound[8]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[8]->p1.x,border + t->bound[8]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[9]->p1.x,border + t->bound[9]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[9]->p1.x,border + t->bound[9]->p1.y,D_TABLE_FLOOR_HEIGHT);

//		glNormal3f(D_SIN_5_PI_4,D_COS_5_PI_4,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[10]->p1.x,border + t->bound[10]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[10]->p1.x,border + t->bound[10]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[11]->p1.x,border + t->bound[11]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[11]->p1.x,border + t->bound[11]->p1.y,D_TABLE_FLOOR_HEIGHT);

//		glNormal3f(0.0,-1.0,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[11]->p1.x,border + t->bound[11]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[11]->p1.x,border + t->bound[11]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[12]->p1.x,border + t->bound[12]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[12]->p1.x,border + t->bound[12]->p1.y,D_TABLE_FLOOR_HEIGHT);

//		glNormal3f(D_SIN_7_PI_4,D_COS_7_PI_4,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[12]->p1.x,border + t->bound[12]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[12]->p1.x,border + t->bound[12]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[13]->p1.x,border + t->bound[13]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[13]->p1.x,border + t->bound[13]->p1.y,D_TABLE_FLOOR_HEIGHT);

//		glNormal3f(D_SIN_3_PI_4,D_COS_3_PI_4,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[14]->p1.x,border + t->bound[14]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[14]->p1.x,border + t->bound[14]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[15]->p1.x,border + t->bound[15]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[15]->p1.x,border + t->bound[15]->p1.y,D_TABLE_FLOOR_HEIGHT);

//		glNormal3f(-1.0,0.0,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[15]->p1.x,border + t->bound[15]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[15]->p1.x,border + t->bound[15]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[16]->p1.x,border + t->bound[16]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[16]->p1.x,border + t->bound[16]->p1.y,D_TABLE_FLOOR_HEIGHT);

//		glNormal3f(D_SIN_5_PI_4,D_COS_5_PI_4,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[16]->p1.x,border + t->bound[16]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[16]->p1.x,border + t->bound[16]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[17]->p1.x,border + t->bound[17]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[17]->p1.x,border + t->bound[17]->p1.y,D_TABLE_FLOOR_HEIGHT);

//		glNormal3f(D_SIN_PI_4,D_COS_PI_4,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[18]->p1.x,border + t->bound[18]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[18]->p1.x,border + t->bound[18]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[19]->p1.x,border + t->bound[19]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[19]->p1.x,border + t->bound[19]->p1.y,D_TABLE_FLOOR_HEIGHT);

//		glNormal3f(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[19]->p1.x,border + t->bound[19]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[19]->p1.x,border + t->bound[19]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[20]->p1.x,border + t->bound[20]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[20]->p1.x,border + t->bound[20]->p1.y,D_TABLE_FLOOR_HEIGHT);

//		glNormal3f(D_SIN_3_PI_4,D_COS_3_PI_4,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[20]->p1.x,border + t->bound[20]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[20]->p1.x,border + t->bound[20]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[21]->p1.x,border + t->bound[21]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[21]->p1.x,border + t->bound[21]->p1.y,D_TABLE_FLOOR_HEIGHT);

//		glNormal3f(D_SIN_PI_4,D_COS_PI_4,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[22]->p1.x,border + t->bound[22]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[22]->p1.x,border + t->bound[22]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[23]->p1.x,border + t->bound[23]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[23]->p1.x,border + t->bound[23]->p1.y,D_TABLE_FLOOR_HEIGHT);

//		glNormal3f(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[23]->p1.x,border + t->bound[23]->p1.y,D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[23]->p1.x,border + t->bound[23]->p1.y,borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[0]->p1.x,border + t->bound[0]->p1.y,borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[0]->p1.x,border + t->bound[0]->p1.y,D_TABLE_FLOOR_HEIGHT);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// draw the border.
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT,GL_AMBIENT,woodHi);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,white);
	glMaterialfv(GL_FRONT,GL_SPECULAR,white);
	glMaterialf (GL_FRONT,GL_SHININESS,D_LIGHT_TABLE_SHININESS);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,t->wood->glID);
	glNormal3f(0.0,0.0,1.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0);
		glVertex3f(0.0,							border + t->bound[2]->p1.y,		borderHeight);
		glTexCoord2f(0.0,1.0);
		glVertex3f(0.0,							border + t->bound[5]->p1.y,		borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[5]->p1.x,	border + t->bound[5]->p1.y,		borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[2]->p1.x,	border + t->bound[2]->p1.y,		borderHeight);

		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[5]->p1.x,	border + t->bound[5]->p1.y,		borderHeight);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[2]->p1.x,	border + t->bound[2]->p1.y,		borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[3]->p1.x,	border + t->bound[3]->p1.y,		borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[4]->p1.x,	border + t->bound[4]->p1.y,		borderHeight);

		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[6]->p1.x,	t->totalHeight,					borderHeight);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[9]->p1.x,	t->totalHeight,					borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[9]->p1.x,	border + t->bound[9]->p1.y,		borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[6]->p1.x,	border + t->bound[6]->p1.y,		borderHeight);

		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[6]->p1.x,	border + t->bound[6]->p1.y,		borderHeight);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[9]->p1.x,	border + t->bound[9]->p1.y,		borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[8]->p1.x,	border + t->bound[8]->p1.y,		borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[7]->p1.x,	border + t->bound[7]->p1.y,		borderHeight);

		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[10]->p1.x,	t->totalHeight,					borderHeight);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[13]->p1.x,	t->totalHeight,					borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[13]->p1.x,	border + t->bound[13]->p1.y,	borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[10]->p1.x,	border + t->bound[10]->p1.y,	borderHeight);

		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[10]->p1.x,	border + t->bound[10]->p1.y,	borderHeight);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[13]->p1.x,	border + t->bound[13]->p1.y,	borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[12]->p1.x,	border + t->bound[12]->p1.y,	borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[11]->p1.x,	border + t->bound[11]->p1.y,	borderHeight);

		glTexCoord2f(0.0,0.0);
		glVertex3f(t->totalWidth,				border + t->bound[14]->p1.y,	borderHeight);
		glTexCoord2f(0.0,1.0);
		glVertex3f(t->totalWidth,				border + t->bound[17]->p1.y,	borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[17]->p1.x,	border + t->bound[17]->p1.y,	borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[14]->p1.x,	border + t->bound[14]->p1.y,	borderHeight);

		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[14]->p1.x,	border + t->bound[14]->p1.y,	borderHeight);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[17]->p1.x,	border + t->bound[17]->p1.y,	borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[16]->p1.x,	border + t->bound[16]->p1.y,	borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[15]->p1.x,	border + t->bound[15]->p1.y,	borderHeight);

		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[18]->p1.x,	0.0,							borderHeight);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[21]->p1.x,	0.0,							borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[21]->p1.x,	border + t->bound[21]->p1.y,	borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[18]->p1.x,	border + t->bound[18]->p1.y,	borderHeight);

		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[18]->p1.x,	border + t->bound[18]->p1.y,	borderHeight);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[21]->p1.x,	border + t->bound[21]->p1.y,	borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[20]->p1.x,	border + t->bound[20]->p1.y,	borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[19]->p1.x,	border + t->bound[19]->p1.y,	borderHeight);

		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[22]->p1.x,	0.0,							borderHeight);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[1]->p1.x,	0.0,							borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[1]->p1.x,	border + t->bound[1]->p1.y,		borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[22]->p1.x,	border + t->bound[22]->p1.y,	borderHeight);

		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[22]->p1.x,	border + t->bound[22]->p1.y,	borderHeight);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[1]->p1.x,	border + t->bound[1]->p1.y,		borderHeight);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[0]->p1.x,	border + t->bound[0]->p1.y,		borderHeight);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[23]->p1.x,	border + t->bound[23]->p1.y,	borderHeight);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// draw the pocket border.
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT,GL_AMBIENT,white);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,white);
	glMaterialfv(GL_FRONT,GL_SPECULAR,white);
	glMaterialf (GL_FRONT,GL_SHININESS,7);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,t->steel->glID);
	glNormal3f(0.0,0.0,1.0);
	// draw the low left pocket border.
	tablePaintCornerPocketBorder3D(t,borderHeight);
	// draw the upper left pocket border.
	glPushMatrix();
		glTranslatef(0.0,t->totalHeight,0.0);
		glRotatef(270,0.0,0.0,1.0);
		tablePaintCornerPocketBorder3D(t,borderHeight);
	glPopMatrix();
	// draw the upper right pocket border.
	glPushMatrix();
		glTranslatef(t->totalWidth,t->totalHeight,0.0);
		glRotatef(180,0.0,0.0,1.0);
		tablePaintCornerPocketBorder3D(t,borderHeight);
	glPopMatrix();
	// draw the lower right pocket border.
	glPushMatrix();
		glTranslatef(t->totalWidth,0.0,0.0);
		glRotatef(90,0.0,0.0,1.0);
		tablePaintCornerPocketBorder3D(t,borderHeight);
	glPopMatrix();
	// draw the upper middle pocket border.
	tablePaintMiddlePocketBorder3D(t,borderHeight);
	// draw the lower middle pocket border.
	glPushMatrix();
		glTranslatef(t->totalWidth,t->totalHeight,0.0);
		glRotatef(180,0.0,0.0,1.0);
		tablePaintMiddlePocketBorder3D(t,borderHeight);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	// draw the table green area.
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT,GL_AMBIENT,greenHi);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,white);
	glMaterialfv(GL_FRONT,GL_SPECULAR,white);
	glMaterialf (GL_FRONT,GL_SHININESS,3);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D,t->field->glID);
	glNormal3f(0.0,0.0,1.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0);
		glVertex3f(border,						border,							D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border,						t->totalHeight - border,		D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(1.0,1.0);
		glVertex3f(t->totalWidth - border,		t->totalHeight - border,		D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(1.0,0.0);
		glVertex3f(t->totalWidth - border,		border,							D_TABLE_FLOOR_HEIGHT);

		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[0]->p1.x,	border + t->bound[0]->p1.y,		D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[1]->p1.x,	border + t->bound[1]->p1.y,		D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[2]->p1.x,	border + t->bound[2]->p1.y,		D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[3]->p1.x,	border + t->bound[3]->p1.y,		D_TABLE_FLOOR_HEIGHT);

		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[4]->p1.x,	border + t->bound[4]->p1.y,		D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[5]->p1.x,	border + t->bound[5]->p1.y,		D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[6]->p1.x,	border + t->bound[6]->p1.y,		D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[7]->p1.x,	border + t->bound[7]->p1.y,		D_TABLE_FLOOR_HEIGHT);

		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[8]->p1.x,	border + t->bound[8]->p1.y,		D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[9]->p1.x,	border + t->bound[9]->p1.y,		D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[10]->p1.x,	border + t->bound[10]->p1.y,	D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[11]->p1.x,	border + t->bound[11]->p1.y,	D_TABLE_FLOOR_HEIGHT);

		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[12]->p1.x,	border + t->bound[12]->p1.y,	D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[13]->p1.x,	border + t->bound[13]->p1.y,	D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[14]->p1.x,	border + t->bound[14]->p1.y,	D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[15]->p1.x,	border + t->bound[15]->p1.y,	D_TABLE_FLOOR_HEIGHT);

		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[16]->p1.x,	border + t->bound[16]->p1.y,	D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[17]->p1.x,	border + t->bound[17]->p1.y,	D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[18]->p1.x,	border + t->bound[18]->p1.y,	D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[19]->p1.x,	border + t->bound[19]->p1.y,	D_TABLE_FLOOR_HEIGHT);

		glTexCoord2f(0.0,0.0);
		glVertex3f(border + t->bound[20]->p1.x,	border + t->bound[20]->p1.y,	D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(0.0,1.0);
		glVertex3f(border + t->bound[21]->p1.x,	border + t->bound[21]->p1.y,	D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(1.0,1.0);
		glVertex3f(border + t->bound[22]->p1.x,	border + t->bound[22]->p1.y,	D_TABLE_FLOOR_HEIGHT);
		glTexCoord2f(1.0,0.0);
		glVertex3f(border + t->bound[23]->p1.x,	border + t->bound[23]->p1.y,	D_TABLE_FLOOR_HEIGHT);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// draw the pockets.
	glShadeModel(GL_FLAT);
	glMaterialfv(GL_FRONT,GL_AMBIENT,zero);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,zero);
	glMaterialfv(GL_FRONT,GL_SPECULAR,zero);
	glMaterialf (GL_FRONT,GL_SHININESS,D_LIGHT_TABLE_SHININESS);
	glNormal3f(0.0,0.0,1.0);
	int	i;
	for ( i = 0; i < TABLE_POCKETS; i++ ) {
		drawCircle3Dxy(pocketRadius,t->pocket[i].x,t->pocket[i].y,D_TABLE_FLOOR_HEIGHT + D_PLANE_DIFF);
	}

	// draw the baulk area.
	glShadeModel(GL_SMOOTH);
	glLineWidth(2.0);
	glMaterialfv(GL_FRONT,GL_AMBIENT,greyHi);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,greyHi);
	glMaterialfv(GL_FRONT,GL_SPECULAR,greyHi);
	glMaterialf (GL_FRONT,GL_SHININESS,D_LIGHT_TABLE_SHININESS);
	TFloat	baulkX = D_TABLE_BAULKLINE_DISTANCE * t->width / D_TABLE_WIDTH;
	glBegin(GL_LINES);
		glNormal3f(0.0,D_COS_3_PI_4,D_SIN_PI_4);
		glVertex3f(border + baulkX,border,D_TABLE_FLOOR_HEIGHT);
		glNormal3f(0.0,D_COS_PI_4,D_SIN_PI_4);
		glVertex3f(border + baulkX,border + t->height,D_TABLE_FLOOR_HEIGHT + D_PLANE_DIFF);
	glEnd();

	// draw the D.
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT,GL_AMBIENT,greyHi);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,greyHi);
	glMaterialfv(GL_FRONT,GL_SPECULAR,greyHi);
	glMaterialf (GL_FRONT,GL_SHININESS,D_LIGHT_TABLE_SHININESS);
	glNormal3f(0.0,0.0,1.0);
	TFloat	baulkR = D_TABLE_BAULK_RADIUS * t->width / D_TABLE_WIDTH;
	drawWiredCircleSection3Dxy(baulkR,border + baulkX,border + t->height / 2.0,D_PI_2,D_3_PI_2,0.1,D_TABLE_FLOOR_HEIGHT + D_PLANE_DIFF);

	glLineWidth(1.0);
	glDisable(GL_DEPTH_TEST);
}

void tablePaint2D(PTable t) {

	TFloat	border = tableGetBorder(t);

	// draw the table border.
	glColor3f(0.34,0.18,0.13);
	glBegin(GL_QUADS);
		glVertex2f(0,0);
		glVertex2f(0,t->totalHeight);
		glVertex2f(t->totalWidth,t->totalHeight);
		glVertex2f(t->totalWidth,0);
	glEnd();

	// draw the table green area.
	glColor3f(0.0,0.4,0.0);
	glBegin(GL_QUADS);
		glVertex2f(border,border);
		glVertex2f(border,t->height + border);
		glVertex2f(t->width + border,t->height + border);
		glVertex2f(t->width + border,border);

		glVertex2f(border + t->bound[0]->p1.x,border + t->bound[0]->p1.y);
		glVertex2f(border + t->bound[1]->p1.x,border + t->bound[1]->p1.y);
		glVertex2f(border + t->bound[2]->p1.x,border + t->bound[2]->p1.y);
		glVertex2f(border + t->bound[3]->p1.x,border + t->bound[3]->p1.y);

		glVertex2f(border + t->bound[4]->p1.x,border + t->bound[4]->p1.y);
		glVertex2f(border + t->bound[5]->p1.x,border + t->bound[5]->p1.y);
		glVertex2f(border + t->bound[6]->p1.x,border + t->bound[6]->p1.y);
		glVertex2f(border + t->bound[7]->p1.x,border + t->bound[7]->p1.y);

		glVertex2f(border + t->bound[8]->p1.x,border + t->bound[8]->p1.y);
		glVertex2f(border + t->bound[9]->p1.x,border + t->bound[9]->p1.y);
		glVertex2f(border + t->bound[10]->p1.x,border + t->bound[10]->p1.y);
		glVertex2f(border + t->bound[11]->p1.x,border + t->bound[11]->p1.y);

		glVertex2f(border + t->bound[12]->p1.x,border + t->bound[12]->p1.y);
		glVertex2f(border + t->bound[13]->p1.x,border + t->bound[13]->p1.y);
		glVertex2f(border + t->bound[14]->p1.x,border + t->bound[14]->p1.y);
		glVertex2f(border + t->bound[15]->p1.x,border + t->bound[15]->p1.y);

		glVertex2f(border + t->bound[16]->p1.x,border + t->bound[16]->p1.y);
		glVertex2f(border + t->bound[17]->p1.x,border + t->bound[17]->p1.y);
		glVertex2f(border + t->bound[18]->p1.x,border + t->bound[18]->p1.y);
		glVertex2f(border + t->bound[19]->p1.x,border + t->bound[19]->p1.y);

		glVertex2f(border + t->bound[20]->p1.x,border + t->bound[20]->p1.y);
		glVertex2f(border + t->bound[21]->p1.x,border + t->bound[21]->p1.y);
		glVertex2f(border + t->bound[22]->p1.x,border + t->bound[22]->p1.y);
		glVertex2f(border + t->bound[23]->p1.x,border + t->bound[23]->p1.y);
	glEnd();

	// draw the pockets.
	TFloat	slice = t->pocketSize / (D_SQRT_2 * 2);

	glColor3f(0,0,0);
	drawCircle2D(t->pocketSize / 2.0,border - D_COS_PI_4 * slice,border - D_SIN_PI_4 * slice);
	drawCircle2D(t->pocketSize / 2.0,border - D_COS_PI_4 * slice,t->height + border + D_SIN_PI_4 * slice);
	drawCircle2D(t->pocketSize / 2.0,t->width + border + D_COS_PI_4 * slice,t->height + border + D_SIN_PI_4 * slice);
	drawCircle2D(t->pocketSize / 2.0,t->width + border + D_COS_PI_4 * slice,border - D_SIN_PI_4 * slice);
	drawCircle2D(t->pocketSize / 2.0,t->totalWidth / 2.0,border - t->pocketSize / 2.0);
	drawCircle2D(t->pocketSize / 2.0,t->totalWidth / 2.0,t->height + border + t->pocketSize / 2.0);

	// draw the baulk area.
	TFloat	baulkX = D_TABLE_BAULKLINE_DISTANCE * t->width / D_TABLE_WIDTH;
	glColor3f(0.7,0.7,0.7);
	glBegin(GL_LINES);
		glVertex2f(border + baulkX,border);
		glVertex2f(border + baulkX,border + t->height);
	glEnd();

	// draw the D.
	TFloat	baulkR = D_TABLE_BAULK_RADIUS * t->width / D_TABLE_WIDTH;
	drawWiredCircleSection2D(baulkR,border + baulkX,border + t->height / 2.0,D_PI_2,D_3_PI_2,0.1);

	#ifdef	DEBUG
		border = tableGetBorder(t);
		glLoadIdentity();
		glBegin(GL_LINES);
			glColor3f(1,0,0);
			int	i = 0;
			for ( i = 0; i < TABLE_BOUNDS; i++ ) {
				glVertex2f(t->bound[i]->p1.x + border,t->bound[i]->p1.y + border);
				glVertex2f(t->bound[i]->p2.x + border,t->bound[i]->p2.y + border);
			}
		glEnd();
	#endif
}

int tableCollided(PTable table, PBall ball) {
	unsigned	hit = 0, i;
	for ( i = 0; i < TABLE_BOUNDS; i++ ) {
		if ( lineBallCollided(table->bound[i],ball) ) {
			hit |= (unsigned)1 << i;
		}
	}
	return hit;
}

int tableInPocket(PTable table, PBall ball, int hit) {
	return hit & table->pockets;
}

TFloat tableCollisionTime(PTable table, PBall ball, int hit) {
	TFloat	time = 0.0;
	int		i;
	for ( i = 0; i < TABLE_BOUNDS; i++ ) {
		if ( hit & 1 ) {
			TFloat	t = 0.0;
			if ( table->bound[i]->p1.x == table->bound[i]->p2.x ) {
				// vertical.
				TFloat	d = ball->radius - distancePointToLineSegment(vertex(ball->x,ball->y),table->bound[i]);
				t = fabs(d / ballSpeedX(ball));
			} else if ( table->bound[i]->p1.y == table->bound[i]->p2.y ) {
				// horizontal.
				TFloat	d = ball->radius - distancePointToLineSegment(vertex(ball->x,ball->y),table->bound[i]);
				t = fabs(d / ballSpeedY(ball));
			} else {
				// rotate.
				TFloat	d, a = -atan(angularCoef(table->bound[i]->p1,table->bound[i]->p2));
				TFloat	mr[2][2];
				TLine	l;
				TVertex	c;
				mr[0][0] = cos(a);
				mr[0][1] = -sin(a);
				mr[1][0] = -mr[0][1];
				mr[1][1] = mr[0][0];
				c.x = mr[0][0] * ball->x + mr[0][1] * ball->y;
				c.y = mr[1][0] * ball->x + mr[1][1] * ball->y;
				l.p1.x = mr[0][0] * table->bound[i]->p1.x + mr[0][1] * table->bound[i]->p1.y;
				l.p1.y = mr[1][0] * table->bound[i]->p1.x + mr[1][1] * table->bound[i]->p1.y;
				l.p2.x = mr[0][0] * table->bound[i]->p2.x + mr[0][1] * table->bound[i]->p2.y;
				l.p2.y = mr[1][0] * table->bound[i]->p2.x + mr[1][1] * table->bound[i]->p2.y;
				// distance.
				d = ball->radius - distancePointToLineSegment(c,&l);
				// time.
				t = fabs(d / (sin(ball->angle - a) * ballSpeed(ball)));
			}
			if ( t > time ) {
				time = t;
			}
		}
		hit = hit >> 1;
	}
	return time;
}

TVector2D tableReflectBall(PLine line, PBall ball) {
	TVector2D	D, B, N, NN, R;
	TVertex		P;
	D = angleToVector(atan((line->p2.y - line->p1.y) / (line->p2.x - line->p1.x)));
	B = angleToVector(ball->angle);
	// normalize B & D.
	vectorNormalize(&D);
	vectorNormalize(&B);
	// normal and inverse of the normal.
	N = vectorNormal(&D);
	NN = vectorNegative(&N);
	// project the collision point at the line.
	P = projectionPointOverLine(vertex(ball->x,ball->y),line);
	// translate.
	P.x = ball->x - P.x;
	P.y = ball->y - P.y;
	// exchange the normal with its inverse.
	if ( distancePointToPoint(vertex(N.i,N.j),P) > distancePointToPoint(vertex(NN.i,NN.j),P) ) {
		N = NN;
	}
	// reflect.
	B = vectorNegative(&B);
	R = vectorReflect(&B,&N);
	return R;
}

TVector2D tableReflectBallOnCorner(TVertex corner, PBall ball) {
	TVector2D	B, N, R;
	B = angleToVector(ball->angle);
	// normal.
	N.i = ball->x - corner.x;
	N.j = ball->y - corner.y;
	// normalize B & N.
	vectorNormalize(&B);
	vectorNormalize(&N);
	// reflect.
	B = vectorNegative(&B);
	R = vectorReflect(&B,&N);
	return R;
}

int tableCollision(PTable table, PBall ball, int hit) {
	if ( !hit ) {
		return 0;
	}
	int	bound = 0, bound2 = 0, i = 0, count = 0;
	for ( i = 0; i < TABLE_BOUNDS; i++ ) {
		if ( hit & 1 ) {
			if ( count >= 1 ) {
				bound2 = i;
			} else {
				bound = i;
			}
			count++;
		}
		hit = hit >> 1;
	}
	TVector2D	R;
	if ( count == 1 ) {
		// somewhere not in a corner.
		R = tableReflectBall(table->bound[bound],ball);
	} else {
		// corner.
		if ( sameVertex(table->bound[bound]->p1,table->bound[bound2]->p2) ) {
			R = tableReflectBallOnCorner(table->bound[bound]->p1,ball);
		} else {
			R = tableReflectBallOnCorner(table->bound[bound]->p2,ball);
		}
	}
	ball->angle = vectorToAngle(&R);
	ball->speed *= D_TABLE_HIT_SPEED_RATIO;
	return hit;
}

TFloat tableGetBorder(PTable table) {
	return (table->totalWidth - table->width) / 2.0;
}

TFloat tableGetAtrCoef(PTable table) {
	return table->atrCoef;
}

TFloat tableGetTotalWidth(PTable table) {
	return table->totalWidth;
}

TFloat tableGetTotalHeight(PTable table) {
	return table->totalHeight;
}

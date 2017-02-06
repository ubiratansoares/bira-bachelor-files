/*
 * textura.c
 *
 * Este programa demonstra o uso das funcionalidades
 *   basicas de textura para versoes de OpneGL superiores
 *   a GL_VERSION_1_0. Ele foi baseado nos programas
 *   texbind.c e texgen.c de distribuicao gratuita.
 *
 * Sao definidas tres unidades de textura (texName) --
 *    1D (padrao tracejado), 2D (padrao xadrez) e 3D (padrao
 *    aleatorio) e foram mapeadas em tres quadrados distintos.
 *
 * 
 * 
 */
 /*http://www.gamedev.net/community/forums/topic.asp?topic_id=500676*/

#include <GL/glut.h>
#include <GL/glext.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static GLuint texName[3];

PFNGLTEXIMAGE3DPROC glTexImage3D;

/* Criar imagem 1D - Padrao Tracejado   */
#define stripeImageWidth 32
GLubyte stripeImage[stripeImageWidth][4];

void PadraoTracejado(void)
{
   int j;

   for (j = 0; j < stripeImageWidth; j++) {
      stripeImage[j][0] = (GLubyte) ((j<=16) ? 255 : 0);
      stripeImage[j][1] = (GLubyte) 120;
      stripeImage[j][2] = (GLubyte) ((j>16) ? 255 : 0);
      stripeImage[j][3] = (GLubyte) 255;
   }
}

/* Criar imagem 2D - Padrao de xadrez   */
#define	checkImageWidth 64
#define	checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

void PadraoXadrez(void)
{
   int i, j, c;

   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = ((((i&0x8)==0)^((j&0x8)==0)))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) 120;
         checkImage[i][j][3] = (GLubyte) 255;
      }
   }
}

/* Criar imagem 3D - Padrao aleatorio   */
#define	woodImageRadius 64

static GLubyte woodImage[woodImageRadius][woodImageRadius][woodImageRadius][4];

void PadraoAleatorio(void)
{
  double radius, angle, grain;
  int i, j, k;

  for (i = 0; i < woodImageRadius; i++) {
      for (j = 0; j < woodImageRadius; j++) {
         for (k = 0; k < woodImageRadius; k++) {
 	    radius = sqrt (i*i + j*j);
            if (k == 0) angle = M_PI_2;
                     else angle = atan2(i,k);
            radius =+ 2*sin(40*angle + k/120.);
            if (radius > 0) {
              /* marrom */
              woodImage[i][j][k][0] = (GLubyte) 150;
              woodImage[i][j][k][1] = (GLubyte) 20;
              woodImage[i][j][k][2] = (GLubyte) 50;
              woodImage[i][j][k][3] = (GLubyte) 255;
            } else {
              /* amarelo */
              woodImage[i][j][k][0] = (GLubyte) 200;
              woodImage[i][j][k][1] = (GLubyte) 200;
              woodImage[i][j][k][2] = (GLubyte) 10;
              woodImage[i][j][k][3] = (GLubyte) 255;
            }
         }
      }
  }
}

void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_DEPTH_TEST);

   /* Obter os IDs dos objetos de textura ainda nao utilizados */
   glGenTextures(3,texName);

   PadraoTracejado();
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   /* Criar um objeto de textura com o padrao Tracejado */
   glBindTexture(GL_TEXTURE_1D, texName[0]);
   /* ... funcao de mapeamento */
   glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   /* ... como o valor dos pixels deve ser modificado */
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   /* ... a imagem */
   glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, stripeImageWidth, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, stripeImage);

   PadraoXadrez();
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   /* Criar um objeto de textura com o padrao Xadrez */
   glBindTexture(GL_TEXTURE_2D, texName[1]);
   /* ... funcao de mapeamento */
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_NEAREST);
   /* ... como o valor dos pixels deve ser modificado */
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   /* ... a imagem */
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
                checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                checkImage);

   PadraoAleatorio();
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   /* Criar um objeto de textura com o padrao Madeira */
   glBindTexture(GL_TEXTURE_3D, texName[2]);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   /* ... como o valor dos pixels deve ser modificado */
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   /* ... a imagem */
   glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, woodImageRadius,
                woodImageRadius, woodImageRadius,0, GL_RGBA, GL_UNSIGNED_BYTE,
                woodImage);
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /* Eh necessario habilitar, um de cada vez, diferentes
      modos de texturizacao */

   glEnable(GL_TEXTURE_1D);
   glBindTexture(GL_TEXTURE_1D, texName[0]);
   glBegin(GL_QUADS);
   /* As coordenadas de textura sao normalizadas em relacao a 32 (extensao
      da textura). No caso, o padrao deveria ser repetido 6 vezes, mas
      foi "grampeado" (CLAMP). */
   glTexCoord1f(0.0); glVertex3f(-2.0, -1.0, 0.0);
   glTexCoord1f(6.0); glVertex3f(-2.0, 1.0, 0.0);
   glTexCoord1f(0.0); glVertex3f(0.0, 1.0, 0.0);
   glTexCoord1f(6.0); glVertex3f(0.0, -1.0, 0.0);
   glEnd();
   glDisable(GL_TEXTURE_1D);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texName[1]);
   glBegin(GL_QUADS);
   /* As coordenadas de textura sao normalizadas em relacao a 64x64 (extensao
      da textura) */
   glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
   glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
   glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
   glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   glEnable(GL_TEXTURE_3D);
   glBindTexture(GL_TEXTURE_3D, texName[2]);
   glBegin(GL_QUADS);
   /* As coordenadas de textura sao normalizadas em relacao a 256x256 (extensao
      da textura). No caso, o padrão de textura é repetido 6 vezes em cada
      direcao (REPEAT). */
   glTexCoord3f(6.0, 0.0, 0.0); glVertex3f(2.5, -1.0, 0.0);
   glTexCoord3f(0.0, 6.0, 0.0); glVertex3f(2.5, 1.0, 0.0);
   glTexCoord3f(0.0, 6.0, 6.0); glVertex3f(4.41421, 1.0, -1.41421);
   glTexCoord3f(6.0, 0.0, 6.0); glVertex3f(4.41421, -1.0, -1.41421);
   glEnd();
   glDisable(GL_TEXTURE_3D);
   glFlush();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(90.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -3.6);
}

/* ARGSUSED1 */
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(250, 250);
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
  
  	// VERY IMPORTANT:
	// this line loads the address of the glTexImage3D function into the function pointer of the same name.
	// glTexImage3D is not implemented in the standard GL libraries and must be loaded dynamically at run time,
	// the environment the program is being run in MAY OR MAY NOT support it, if not we'll get back a NULL pointer.
	// this is necessary to use any OpenGL function declared in the glext.h header file
	// the Pointer to FunctioN ... PROC types are declared in the same header file with a type appropriate to the function name
	glTexImage3D = (PFNGLTEXIMAGE3DPROC) wglGetProcAddress("glTexImage3D");
	if (glTexImage3D == NULL) {
		printf("Error in line %d: Couldn't load glTexImage3D function. Aborting.\n", __LINE__);
		return -1;
	}

   
   init();
   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc (keyboard);
   glutMainLoop();
   return 0;
}

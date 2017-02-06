/*http://www.ceset.unicamp.br/~magic/opengl/textura-simples.html*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "image.h"

/*Define as cores e coordenadas das texturas do plano, além dos nomes dos 
arquivos que contém as imagens das texturas.*/
#define COORD_TEXTURA_PLANO 1.0
#define COR_DO_PLANO 0.52,0.52,0.78,1.0
#define TEXTURA_DO_PLANO "montanhas.rgb"

/*Quando uma textura é carregada, o OpenGL guarda em uma matriz e assume 
coordenadas (0,0), (1,0), (1,1) e (0,1) para os quatro cantos da textura. 
Neste exemplo, é assumido a repetição das texturas, de modo a poder cobrir todo 
o objeto. As coordenadas especificadas neste trecho de código indicam que o 
objeto será carimbado com sua respectiva textura e que as coordenadas das 
texturas que será usada para criar o carimbo do plano é ctp[]. 
*/
GLfloat ctp[4][2]={
  {-COORD_TEXTURA_PLANO,-COORD_TEXTURA_PLANO},
  {+COORD_TEXTURA_PLANO,-COORD_TEXTURA_PLANO},
  {+COORD_TEXTURA_PLANO,+COORD_TEXTURA_PLANO},
  {-COORD_TEXTURA_PLANO,+COORD_TEXTURA_PLANO}
};

GLuint  textura_plano;  //A variável textura_plano armazena o identificador da textura do plano.
static int shoulder = 0, elbow = 0, elbow2 = 0;



void display(void){

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    /* habilita/desabilita uso de texturas*/
  glEnable(GL_TEXTURE_2D);

  glColor4f(COR_DO_PLANO);
/*glTexEnvf define os parâmetros do ambiente de textura. O primeiro parâmetro é sempre 
GL_TEXTURE_ENV; o segundo parâmetro é sempre GL_TEXTURE_ENV_MODE e o terceiro 
especifica como a textura será combinada com a cor para formar a superfície do 
objeto, neste caso substituindo completamente a cor do objeto pela textura corrente.
Outros valores: GL_MODULATE, GL_DECAL, GL_BLEND, or GL_REPLACE
*/  
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
  /* glBindTexture() carrega a textura bidimensional associada com a variável textura_plano. 
  Em seguida, um plano é desenhado usando GL_QUADS. Observe que, antes de desenhar 
  cada vértice, a função glTexCoord2fv() é chamada para definir as coordenadas 
  de textura correntes.
*/
  glBindTexture(GL_TEXTURE_2D,textura_plano);
   
  glBegin(GL_QUADS);
  glTexCoord2fv(ctp[0]);  glVertex3f(-5,-3.2,5);
  glTexCoord2fv(ctp[1]);  glVertex3f(5,-3.2,5);
  glTexCoord2fv(ctp[2]);  glVertex3f(5,-3.2,-5);
  glTexCoord2fv(ctp[3]);  glVertex3f(-5,-3.2,-5);
  glEnd();

  glDisable(GL_TEXTURE_2D);  // Desabilita o uso de textura. Passa a ser usado o a cor de glColor

  glPushMatrix();


  glRotatef ((GLfloat) shoulder, 0.0, 1.0, 0.0);

  /* Chao*/
  glColor3f(0.0,0.0,1.0);
  glPushMatrix();
  glTranslatef (0.0, -3.4, 0.0);
  glScalef (10.0, 0.2, 10.0);
  glutSolidCube (1.0);
  glPopMatrix();

  /* armario */
  glColor3f(1.0,0.0,1.0);
  glPushMatrix();
  glScalef (4.0, 6.0, 2.0);
  glutSolidCube (1.0);
  glPopMatrix();

  /* porta 1*/

  glColor3f(1.0,1.0,0.0);
  glPushMatrix();
  glTranslatef (-2.0, 0.0, 1.1);
  glRotatef ((GLfloat) elbow, 0.0, 1.0, 0.0);
  glTranslatef (2.0, 0.0, -1.1);
  glTranslatef (-1.0, 0.0, 1.0);
  glScalef (2.0, 6.0, 0.2);
  glutSolidCube (1.0);
  glColor3f(1.0,0.0,0.0);
  glPushMatrix();    /* desenha macaneta 1*/
  glTranslatef (0.4, 0.0, 1.1);
  glScalef (0.2, 0.2, 0.2);
  glutSolidCube (1.0);
  glPopMatrix();
  glPopMatrix();

/* porta 2*/

  glColor3f(1.0,1.0,0.0);
  glPushMatrix();
  glTranslatef (2.0, 0.0, 1.1);
  glRotatef ((GLfloat) elbow2, 0.0, 1.0, 0.0);
  glTranslatef (-2.0, 0.0, -1.1);
  glTranslatef (1.0, 0.0, 1.0);
  glScalef (2.0, 6.0, 0.2);
  glutSolidCube (1.0);
  glColor3f(1.0,0.0,0.0);
  glPushMatrix();    /* desenha macaneta 2*/
  glTranslatef (-0.4, 0.0, 1.1);
  glScalef (0.2, 0.2, 0.2);
  glutSolidCube (1.0);
  glPopMatrix();
  glPopMatrix();

  /* origem volta para o sistema de coordenadas original */
  glPopMatrix();
  glutSwapBuffers();
}

/*A carga da textura é feita com uso da função ImageLoad(), implementada em 
image.c e definida em image.h. Esta função recebe como parâmetro o nome do 
arquivo com a imagem da textura e retorna um ponteiro para uma estrutura de 
dados do tipo IMAGE.
*/
void carregar_texturas(void){
  IMAGE *img;
  GLenum gluerr;

/*glGenTextures() gera 1 nome de textura em textura_plano. Um nome de textura é 
qualquer inteiro diferente de zero que identifique de forma única a textura. 
Assim, texturas diferentes possuem nomes diferentes. Caso o segundo parâmetro 
desta função seja um vetor de elementos GLuint, mais nomes de textura podem ser 
gerados, um para cada elemento deste vetor.*/
  /* textura do plano */
  glGenTextures(1, &textura_plano);
  glBindTexture(GL_TEXTURE_2D, textura_plano);
  
  if(!(img=ImageLoad(TEXTURA_DO_PLANO))) {
    fprintf(stderr,"Error reading a texture.\n");
    exit(-1);
  }

/*Mipmaps são séries de versões em baixa resolução de um mapa de textura. 
Geralmente é utilizado para texturizar um objeto cuja resolução na tela difere 
da resolução no mapa de textura. Por exemplo, um objeto próximo da tela pode ser 
desenhado utilizando uma resolução de textura maior que um objeto distante da 
tela. O uso de mipmaps evita o efeito de serrilhamento (aliasing) e outros 
distúrbios de exibição quando um objeto é aproximado ou afastado da tela.

Mipmaps bidimensionais são construídos com a função gluBuild2dMipmaps(), que 
possuiu seguinte protótipo: 
void gluBuild2dMipmaps(GLenum target, GLint internalFormat, GLsizei width, 
                       GLsizei height, GLenum format, GLenum type, const void *data);

target indica o tipo de mipmaps que se deseja construir - neste caso uma textura bidimensional. 
internalFormat indica o tipo de armazenagem interna do arquivo de textura. 
               Neste exemplo, a textura é uma imagem RGB, incluindo 3 (três) componentes de cor. 
Os parâmetros width e height especificam a largura e a altura do dado. Ambas estas dimensões devem ser potências de 2. 
format especifica o formato dos pixels do dado (RGB). 
type especifica o tipo de dado representado no vetor de dados. 
data especifica o ponteiro para a posição de memória onde os dados de textura residem. 

*/
  gluerr=gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 
			   img->sizeX, img->sizeY, 
			   GL_RGB, GL_UNSIGNED_BYTE, 
			   (GLvoid *)(img->data));
  if(gluerr){
    fprintf(stderr,"GLULib%s\n",gluErrorString(gluerr));
    exit(-1);
  }

/*A função glTexParameterf() define uma série de parâmetros que controlam como 
uma textura é tratada e aplicada a um fragmento de um dado objeto. Aqui, através 
dos parâmetros GL_TEXTURE_WRAP_S e GL_TEXTURE_WRAP_T, a função define que nas 
direções s e t (coordenadas) a textura deverá será repetida no objeto.
*/
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

/*Neste trecho do código, as funções glTexParameterf() define os tipos de filtro 
usados quando a textura for minimizada ou maximizada. Pelo parâmetro 
GL_LINEAR_MIPMAP_LINEAR, o OpenGL escolhe dois mipmaps que mais aproximam o 
tamanho do pixel a ser texturizado calcula a média dos quatro elementos de 
textura mais próximos do centro do pixel. O valor da textura para o pixel será a 
média desses dois valores. O parâmetro GL_LINEAR, por sua vez não utiliza 
mipmaps: associa ao pixel a ser texturizado a média dos quatro elementos de 
textura mais próximos do centro do pixel.
*/
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

}

void reshape (int w, int h){
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef (0.0, 0.0, -15.0);
}

void keyboard (unsigned char key, int x, int y){
  switch (key) {
  case 's':
    shoulder = (shoulder + 5) % 360;
    glutPostRedisplay();
    break;
  case 'S':
    shoulder = (shoulder - 5) % 360;
    glutPostRedisplay();
    break;
  case 'e':
    elbow = (elbow + 5) % 360;
    glutPostRedisplay();
    break;
  case 'E':
    elbow = (elbow - 5) % 360;
    glutPostRedisplay();
    break;
  case 'p':
    elbow2 = (elbow2 + 5) % 360;
    glutPostRedisplay();
    break;
  case 'P':
    elbow2 = (elbow2 - 5) % 360;
    glutPostRedisplay();
    break;
  case 27:
    exit(0);
    break;
  default:
    break;
  }
}

void init(void){

  glClearColor (1.0, 1.0, 1.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  carregar_texturas();
  //glEnable(GL_TEXTURE_2D);
}

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  //glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);
  init ();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0; 
}

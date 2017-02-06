// PrimeiroPrograma.c - Isabel H. Manssour
// Um programa OpenGL simples que abre uma janela GLUT
// Este código está baseado no Simple.c, exemplo 
// disponível no livro "OpenGL SuperBible", 
// 2nd Edition, de Richard S. e Wright Jr.

#include <GL/glut.h>

GLfloat fAspect = 1.0f;

// Função callback chamada para fazer o desenho
void Desenha(){ 
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0f, 0.0f, 0.0f);
  
   glBegin(GL_QUADS);
      glVertex2f(-45.0f, -15.0f);
      glVertex2f(-45.0f, 15.0f);
      glColor3f(1.0f, 1.0f, 1.0f);
      glVertex2f(-15.0f, 15.0f);
      glVertex2f(-15.0f, -15.0f);
   glEnd();

  glFlush();     
}

// Inicializa parâmetros de rendering
void Inicializa (void)
{   
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60,fAspect,0.5,500);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(40,60,100, 0,0,0, 0,1,0);
}


void alteraTamanhoJanela(GLsizei w, GLsizei h) {
  GLsizei largura, altura;

  largura = w;
  altura = h;

  glViewport(0, 0, largura, altura);

  
}

// Programa Principal 
int main(int argc, char* argv[])
{
        glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Primeiro Programa");
	glutDisplayFunc(Desenha);
	gluOrtho2D(-50, -10, -20, 20);
        Inicializa();
	glutMainLoop();
}


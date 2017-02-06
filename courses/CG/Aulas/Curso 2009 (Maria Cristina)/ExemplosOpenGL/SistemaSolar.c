#include <GL/glut.h>
#include <stdio.h>


static int year = 0, day = 0, wire = 0;

/* Define o modelo de cores a ser utilizado 
   GL_FLAT : a cor não varia na primitiva que é desenhada */

void init(void)
{
  glClearColor (0.0, 0.0, 0.0, 0.0); 
  //glShadeModel (GL_FLAT);
  glEnable (GL_DEPTH_TEST);

/*
   GLfloat light_pos[] = {1.0, 1.0, 1.0, 1.0};
   GLfloat light_color[] = {1.0, 1.0, 1.0, 1.0};

   GLfloat mat_specular[] = {0.5, 0.5, 0.5, 1.0};
   GLfloat mat_shininess[] = {1.0, 1.0, 1.0, 1.0};

   // Determinando o modelo de renderização
   glShadeModel(GL_SMOOTH);

   // Configurando a luz
   glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);

   // Material default
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);

   // Ativando
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);*/


}

/*
  Função responsável pelo desenho das esferas.
  Nesta função também serão aplicadas as tranformações
  necessárias para o efeito desejado.
*/

void display(void)
{
  //static bool rotacionou_plutao = false;

  /*
    Limpa o buffer de pixels e 
    determina a cor padrão dos objetos.
  */  
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f (1.0, 1.0, 1.0);

    
  //SOL
    glPushMatrix();
      glColor3f (0.89, 0.79, 0.09);
	  //glColor3f (0.89, 0.79, 0.09);
      //glTranslatef(0.0, 0.0, 2.0);     
	  glRotatef ((GLfloat) year, 1.0, 0.0, 0.0);         		  
      if (wire == 0)      
         glutSolidSphere(0.3, 20, 16);		 
      else
         glutWireSphere(0.3, 20, 16);
      glTranslatef(0.0, 0.0, -2.0);     
    
   glPopMatrix();


   //MERCURIO
   glPushMatrix();
      glRotatef ((GLfloat) year * 1.3, 0.0, 1.0, 0.0);
      glTranslatef (0.55, 0.0, 0.0);
      glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);
      glColor3f (1.0, 0.0, 0.0);

      if (wire == 0)
          glutSolidSphere(0.05, 10, 8); 
      else
          glutWireSphere(0.05,10, 8);

    glPopMatrix();


   //VENUS
   glPushMatrix();
      glRotatef ((GLfloat) year * 1.1, 0.0, 1.0, 0.0);
      glTranslatef (0.75, 0.0, 0.0);
      glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);
      glColor3f (0.8, 0.4, 0.2);

      if (wire == 0)
          glutSolidSphere(0.068, 10, 8); 
      else
          glutWireSphere(0.068,10, 8);
   glPopMatrix();



  //TERRA
   glPushMatrix();
      glRotatef ((GLfloat) year, 0.0, 1.0, 0.0);
      glTranslatef (1.0, 0.0, 0.0);
      glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);
      glColor3f (0.2, 0.2, 1.0);

      if (wire == 0)
          glutSolidSphere(0.08, 10, 8); 
      else
          glutWireSphere(0.08,10, 8);

    glPopMatrix();


  //LUA
   glPushMatrix();      
      
      glRotatef ((GLfloat) year, 0.0, 1.0, 0.0);      
	  glTranslatef (1.0, 0.0, 0.0);  
      glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);      
	  glColor3f (1.0, 1.0, 1.0);

	  //Translacao em torno da Terra
	  glRotatef ((GLfloat) year, 0.0, 0.0, 1.0);
	  glTranslatef (0.0, 0.0, 0.108);
      
      
      if (wire == 0)
          glutSolidSphere(0.0216, 10, 8); 
      else
          glutWireSphere(0.0216,10, 8);

	glPopMatrix();




    //MARTE
    glPushMatrix();
      glRotatef ((GLfloat) year / 1.3, 0.0, 1.0, 0.0);
      glTranslatef (1.2, 0.0, 0.0);
      glRotatef ((GLfloat) day , 0.0, 1.0, 0.0);
      glColor3f (0.6, 0.0, 0.0);

      if (wire == 0)
          glutSolidSphere(0.08, 10, 8); 
      else
          glutWireSphere(0.08,10, 8);

    glPopMatrix();


    //JUPITER
    glPushMatrix();
      glRotatef ((GLfloat) year / 1.5, 0.0, 1.0, 0.0);
      glTranslatef (1.7, 0.0, 0.0);
      glRotatef ((GLfloat) day , 0.0, 1.0, 0.0);
      glColor3f (0.5, 0.0, 0.0);

      if (wire == 0)
          glutSolidSphere(0.1, 10, 8); 
      else
          glutWireSphere(0.1,10, 8);
    glPopMatrix();


    //SATURNO
    glPushMatrix();
      glRotatef ((GLfloat) year / 1.7, 0.0, 1.0, 0.0);
      glTranslatef (2.0, 0.0, 0.0);
      glRotatef ((GLfloat) day , 0.0, 1.0, 0.0);
      glColor3f (0.5, 0.3, 0.0);

      if (wire == 0)
          glutSolidSphere(0.091, 10, 8); 
      else
          glutWireSphere(0.091,10, 8);
    glPopMatrix();


    //URANO
    glPushMatrix();
      glRotatef ((GLfloat) year / 2.0, 0.0, 1.0, 0.0);
      glTranslatef (2.3, 0.0, 0.0);
      glRotatef ((GLfloat) day , 0.0, 1.0, 0.0);
      glColor3f (0.1, 0.3, 0.6);

      if (wire == 0)
          glutSolidSphere(0.061, 10, 8); 
      else
          glutWireSphere(0.061,10, 8);
    glPopMatrix();


	//NETUNO
    glPushMatrix();
      glRotatef ((GLfloat) year / 2.3, 0.0, 1.0, 0.0);
      glTranslatef (2.5, 0.0, 0.0);
      glRotatef ((GLfloat) day , 0.0, 1.0, 0.0);
      glColor3f (0.1, 0.1, 0.6);

      if (wire == 0)
          glutSolidSphere(0.051, 10, 8); 
      else
          glutWireSphere(0.051,10, 8);
    glPopMatrix();


	//PLUTAO
    glPushMatrix();           
      //rotaciona plutao em torno do eixo Z. Plutao tem uma translacao 'inclinada'
	  glRotatef (10, 0.0, 0.0, 1.0);      
	  glRotatef ((GLfloat) year / 2.7, 0.0, 1.0, 0.0);	
      glTranslatef (2.8, 0.0, 0.0);
      glRotatef ((GLfloat) day , 0.0, 1.0, 0.0);
      glColor3f (0.1, 0.5, 0.8);

      if (wire == 0)
          glutSolidSphere(0.031, 10, 8); 
      else
          glutWireSphere(0.031,10, 8);
    glPopMatrix();



  // Executa os comandos 
  glutSwapBuffers();

}

/* 
   Função responsável pelo desenho da tela
   Nesta função são determinados o tipo de Projeção
   o modelo de Matrizes e
   a posição da câmera  
   Quando a tela é redimensionada os valores 
   da visão perspectiva são recalculados com base no novo tamanho da tela
   assim como o Viewport
*/   

void reshape (int w, int h)
{

  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

/* Função responsável pelo controle de teclado
   quando pressionada a tecla d, será executada uma rotação no
   próprio eixo da esfera menor. Quando pressionada a tecla y
   a esfera menor irá rotacionar em torno da esfera maior, em uma
   órbida determinada na translação na função display()
   A tecla w é responsável por determinar se as esferas serão sólidas
   ou aramadas (wire)
*/   

void keyboard (unsigned char key, int x, int y)
{
  static float camera_x = 0.0, 
	           camera_y = 0.0, 
			   camera_z = 3.0;


  switch (key) {
      
      case 'a':
		   glRotatef (10, 0.0, 0.0, 1.0);
           break;
      case '4':
		   glRotatef (10, 0.0, 0.0, 1.0);
           break;	  
      case '6':
		   glRotatef (-10, 0.0, 0.0, 1.0);
           break;
	  case '8':
		   glRotatef (10, 1.0, 0.0, 0.0);
           break;
	  case '2':
		   glRotatef (-10, 1.0, 0.0, 0.0);
           break;
      case '-':
		   glTranslatef (0.0, 0.0, -1.0);
           break;
      case '+':
		   glTranslatef (0.0, 0.0, 1.0);
           break;
      case 'd':
           glLoadIdentity();
		   //gluLookAt (0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		   gluLookAt (camera_x, camera_y, camera_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
           camera_z += 1.0;
		   break;
      case 'D':
           day = (day + 10) % 360;
           glutPostRedisplay();
           break;
      case 'Y':
      case 'y':
           year = (year + 5) % 360;
           glutPostRedisplay();
           break;
      case 'w' :
      case 'W' :
           wire = (wire == 1 ? 0 : 1);
           glutPostRedisplay();
           break; 
      default:
           break;
  }
}

/*
  Esta função é chamada quando o botão esquerdo do 
  mouse é pressionado, a mesma irá calcular um novo
  valor para os valores dos ângulos contidos em year e day  
*/
void spinDisplay(void)
{
  year = (year + 1) /*% 360*/;
  day  = (day + 2 ) /*% 360*/;
  glutPostRedisplay();
}


/* 
  Esta função irá controlar os botões do mouse.
  Se pressionado o botão da esquerda ela define
  a função spinDisplay como a função de "idle" do GLUT
  o comando glutIdelFunc, executa uma determinada função quando 
  nenhum evento estiver ocorrendo. (pressionamento de botões etc.)
  Quando o botão do meio é pressionado a função de Idle recebe NULL
  desabilitando a animação
*/
void mouse(int button, int state, int x, int y)
{
  switch (button) {
      case GLUT_LEFT_BUTTON:
           if (state == GLUT_DOWN)
              glutIdleFunc(spinDisplay);
           break;
      case GLUT_MIDDLE_BUTTON:
           if (state == GLUT_DOWN)
              glutIdleFunc(NULL);
           break;
      default:
           break;
  }
}

/*
 Função principal do programa.
*/
int main(int argc, char** argv)
{
  
  glutInit(&argc, argv);  
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  //glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100);
  glutCreateWindow ("Sistema Solar");
  init ();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}

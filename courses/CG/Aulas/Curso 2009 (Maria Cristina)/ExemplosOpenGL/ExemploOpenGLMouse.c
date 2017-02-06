/*
* Danilo Medeiros Eler
* danilome@gmail.com
*/
#include <GL/glut.h>
#include <stdlib.h>
GLint cont;
char cX[10], cY[10], title[10];
GLint pSize;
void redesenha(){  
/*
   glPointSize(10);
   glBegin(GL_POINTS);
 	  glVertex2f(-0.5,-0.5);
   glEnd();  
*/
   
 glFlush();
}

void myKeyboard(unsigned char key, GLint x, GLint y){
 switch(key){    
   case 'c':
           glClearColor(0.0,0.0,0.0,0.0);
           glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
           break;
   case '+':
            pSize++;
            glPointSize(pSize);
            break;
   case '-':
            pSize--;
            if (pSize <= 0) pSize = 1;
            glPointSize(pSize);            
            break;                    
   case 27: //ESC
   case 'q':     
            exit(-1);
            break;
   case 'a':glutFullScreen(); break;
   case 'A':glutReshapeWindow(400, 400); break; 
 }
 glFlush();   
} 

void myMouse(GLint button, GLint state, GLint x, GLint y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        glColor3f(1.0,0,0); 
		glBegin(GL_POINTS);
 	      glVertex2f(x,y);
        glEnd();       
        glFlush();
    }	
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		exit(-1);
}


void myMovedMouse(GLint x, GLint y){
 /*  itoa(x, cX, 10);
   itoa(y, cY, 10);
   strcpy( title, cX);
   strcat(title," ");
   strcat(title,cY);
   glutSetWindowTitle( title );*/
   
   glColor3f(0.0,1.0,0);
   glBegin(GL_POINTS);
 	  glVertex2f(x,y);
   glEnd();
   glFlush();
}

int main(int argc, char *argv[])
{
    pSize = 1;
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(400,400); 
    glutInitWindowPosition(1,1);
    glutCreateWindow("Mouse");
    gluOrtho2D(0,399,399,0); //399
    glutPassiveMotionFunc(NULL);
    glutMotionFunc(myMovedMouse);    
    glutMouseFunc(myMouse);
    
    glutKeyboardFunc(myKeyboard); 
    glutDisplayFunc(redesenha);
    
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     
    glPointSize(pSize);
    glutMainLoop();


    return EXIT_SUCCESS;
}

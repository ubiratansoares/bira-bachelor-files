// HELLO WORD em OpenGl
// Para compilar
// ~>gcc -o teste openglteste.c -framework OpenGL -framework GLUT


#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>

//Let us create a function to initialize everything

void myInit(void){
	glClearColor(1.0,1.0,1.0,1.0);//This determines the background color, in this case White
	glMatrixMode(GL_PROJECTION);//Determines which matrix to apply operations to. Don’t worry about this for now just assume it has to be there
	glLoadIdentity();//Loads the Identity matrix
	gluOrtho2D(0, 640, 0, 480);//Set the size and projection of the buffer
}

//Now, lets tell it to display some stuff

void myDisplay(void){

	glClear(GL_COLOR_BUFFER_BIT);//Clear the buffer
	glColor3d(1,0,0);//Set the color for this polygon
	glBegin(GL_POLYGON);//Let us begin drawing some points

	//Specify the points
 	glVertex2i(100,50);
 	glVertex2i(100,130);
 	glVertex2i(150, 130);
 	glVertex2i(100,50);
 	glEnd();//Ok we are done specifying points
 	glFlush();//Write this out to the screen
}

int main (int argc, char **argv) {

	glutInit(&argc,argv);//Init glut passing some args, if you know C++ you should know we are just passing the args straight thru from main
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);//Specify the Display Mode, this one means there is a single buffer and uses RGB to specify colors
	glutInitWindowSize(640, 480);//Set the window size
	glutInitWindowPosition(100,100);//Where do we want to place the window initially?
	glutCreateWindow("My First Window");//Name the window and create it
	glutDisplayFunc(myDisplay);//Set the callback function, will be called as needed
	myInit();//Initialize the window
	glutMainLoop();//Start the main loop running, nothing after this will execute for all eternity (right now
	return 0;//Return from this function.
}
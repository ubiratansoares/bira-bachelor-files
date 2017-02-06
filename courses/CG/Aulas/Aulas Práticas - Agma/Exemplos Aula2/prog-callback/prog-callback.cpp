/* 
* Programa que apresenta as transformações aplicadas a uma primitiva
* Objetivo : Demonstrar a utilização de transformação de objetos
* Referência do Código: OpenGL Programming Guide - RedBook
*/

#include <GL/gl.h>
#include <GL/glut.h>

GLfloat escala;
GLfloat translada;
GLfloat rotaciona;

void display(void)
{
    glMatrixMode(GL_MODELVIEW);
    
    /*
    Faz com que a matriz corrente seja inicializada com a matriz identidade
    (nenhuma transformação é acumulada)
    */
    glLoadIdentity();

    /* Limpa o Buffer de Pixels */
    glClear(GL_COLOR_BUFFER_BIT);
    
    /* Estabelece a cor da primitiva */
    glColor3f (1.0f,1.0f,1.0f);
    
    /* Efetua a operação de translação */
    glTranslatef(translada, 0.0f, 0.0f);
    
    /* Efetua a operação de escala em Y */
    glScalef(1.0f, escala, 1.0f);
    
    /* Efetua a operação de rotação em Z */
    glRotatef(rotaciona, 0.0f, 0.0f, 1.0f);
    
    /* desenha um simples retângulo */
    glBegin(GL_QUADS);
    glVertex3f (0.025, 0.025, 0.0);
    glVertex3f (0.075, 0.025, 0.0);
    glVertex3f (0.075, 0.075, 0.0);
    glVertex3f (0.025, 0.075, 0.0);
    glEnd();
    
    /* Inicia o processo de desenho através dos
    dados bufferizados
    */
    glFlush ();
}
void init (void)
{

    escala = 1;
    translada = 0;
    rotaciona = 0;

/* Seleciona a cor de fundo para limpeza da tela */
   glClearColor (0.0, 0.0, 0.0, 0.0);
}

/*
Função responsável pelo controle do teclado. Dependendo da tecla pressionada :
R,S,T, irá efetuar respectivamente as operações de
Rotação, Escala e Translação
*/
void teclado(unsigned char tecla, int x, int y)
{
    switch (tecla) {
        case 'S':
        case 's':// Incrementa o valor do parâmetro de escala
            escala = escala + 0.5;
        break;
        case 'T':
        case 't':// Incrementa o valor do parâmetro de translacao
            translada = translada + 0.05;
        break;
        case 'R':
        case 'r':// Incrementa o valor do ângulo de rotação
            rotaciona = rotaciona - 5.0;
        break;
        case 'O':
        case 'o':
            translada = 0.0;
            escala = 1.0;
            rotaciona = 0;
        break;
    }
    glutPostRedisplay();
}

/*
Funcão principal do programa.
*/
int main(int argc, char** argv)
{
    /*
    Estabelece o modo de exibição a ser utilizado pela janela a ser
    neste caso utiliza-se de um buffer simples, ou seja, a apresentação
    execução
    Define o modo de cores como RGBA
    */
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    
    /*
    Determina o tamanho em pixels da
    janela a ser criada
    */
    glutInitWindowSize (500, 500);
    
    /*
    Estabelece a posição inicial para criação da
    janela
    */
    glutInitWindowPosition (100, 100);
    
    /*
    Cria uma janela com base nos parâmetros especificados
    nas funções glutInitWindowSize e glutInitWindowPosition
    com o nome de título especificado em seu argumento
    */
    glutCreateWindow ("Exemplo - Transformações");
    
    /*
    Habilita a captura dos eventos de teclado
    */
    glutKeyboardFunc(teclado);
    
    /*
    Especifica os parâmetros inicias para as variáveis
    de estado do OpenGL
    */
    init ();
    
    // Associa a função display como uma função de callback
    glutDisplayFunc(display);
    
    /*
    Inicia a execução do programa OpenGL.
    O programa irá executar num loop infinito devendo
    o desenvolvedor especificar as condições de saída do mesmo
    através de interrupções no próprio programa ou através
    de comandos de mouse ou teclado como funções de callback
    */
    glutMainLoop();
    return 0;
}

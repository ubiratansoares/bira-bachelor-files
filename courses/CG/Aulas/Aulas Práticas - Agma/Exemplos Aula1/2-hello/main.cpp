/* hello.c 
* Programa simples que apresenta o desenho de um quadrado
* Objetivo : Demonstrar funções de gerenciamento de
* janelas e funções de callback
* Referência do Código: OpenGL Programming Guide - RedBook
*/

#include <GL/gl.h>
#include <GL/glut.h>

void display(void)
{
    /* Limpa o Buffer de Pixels */
    glClear (GL_COLOR_BUFFER_BIT);
    
    // Define a cor padrão como branco
    glColor3f (1.0, 1.0, 1.0);
    
    /* desenha um simples retângulo com as coordenadas
    * (0.25, 0.25, 0.0) and (0.75, 0.75, 0.0)
    */
    glBegin(GL_POLYGON);
    glVertex3f (0.25, 0.25, 0.0);
    glVertex3f (0.75, 0.25, 0.0);
    glVertex3f (0.75, 0.75, 0.0);
    glVertex3f (0.25, 0.75, 0.0);
    glEnd();
    
    /* Inicia o processo de desenho através dos
    dados bufferizados
    */
    glFlush ();
}

void init (void)
{
    /* Seleciona a cor de fundo para limpeza da tela */
    glClearColor (0.0, 0.0, 0.0, 0.0);
    
    /* inicializa os valores de visualização */
    glMatrixMode(GL_PROJECTION);
    
    /* Faz com que a matriz corrente seja inicializada com a matriz identidade
    (nenhuma transformação é acumulada)
    */
    //glLoadIdentity();
    //glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

/*
Cria a janela
*/
int main(int argc, char** argv)
{
    /* Estabelece o modo de exibição a ser utilizado pela janela a ser criada
    neste caso utiliza-se de um buffer simples, ou seja, a apresentação será imediata à
    execução
    Define o modo de cores como RGBA */
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    
    /*Determina o tamanho em pixels da
    janela a ser criada*/
    glutInitWindowSize (250, 250);
    
    /*Estabelece a posição inicial para criação da
    janela*/
    glutInitWindowPosition (100, 100);
    
    /* Cria uma janela com base nos parâmetros especificados
    nas funções glutInitWindowSize e glutInitWindowPosition
    com o nome de título especificado em seu argumento */
    glutCreateWindow ("Exemplo 1");

    /* Especifica os parâmetros inicias para as variáveis
    de estado do OpenGL*/
    init ();

    // Associa a função display como uma função de callback
    glutDisplayFunc(display);

    /* Inicia a execução do programa OpenGL.
    O programa irá executar num loop infinito devendo
    o desenvolvedor especificar as condições de saída do mesmo
    através de interrupções no próprio programa ou através
    de comandos de mouse ou teclado como funções de callback*/
    glutMainLoop();
    
    return 0;
}


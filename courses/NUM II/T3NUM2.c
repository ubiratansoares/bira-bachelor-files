/*****************************************************************************/
/***              TRABALHO 3 - SCE0101 - CÁLCULO NUMÉRICO II               ***/  
/*****************************************************************************/

/***********************************************
*****   TRABALHO DESENVOLVIDO PELOS ALUNOS :
*****   Ubiratan Soares(5634292)
*****   Thiago Fontes Machado (2373831)
***********************************************/


#include <stdio.h>

#define MAX 300 // Tamanho máximo do lado da região retangular de integração

typedef struct{
               float x;   // Essa estrutura é definida de maneira a representar
               float fx;  // o par (x,f(x,y)), com y fixado. Essa notação é
               }point;    // mais limpa do que a similar com matrizes.

// Função : Simpsom13(A,k).
// Essa função aceita o vetor com de pontos (x,f(x,y)) e um inteiro k e
// aproxima a integral da função f através da técnica 1/3 de Simpsom.
               
float Simpson13(point A[], int k)
{
 float aux;
 float h;
 
 aux = A[k].fx + 4*A[k+1].fx + A[k+2].fx;
 h = A[k+1].x - A[k].x; 
 
 return(h*aux/3); 
}


// Função : Simpsom38(A,k).
// Essa função aceita o vetor com de pontos (x,f(x,y)) e um inteiro k e
// aproxima a integral da função f através da técnica 3/8 de Simpsom.

float Simpson38(point A[],int k)
{
 float aux;
 float h;
  
 aux = A[k].fx + 3*A[k+1].fx + 3*A[k+2].fx + A[k+3].fx;
 h = A[k+1].x - A[k].x;
 
 return(h*3*aux/8);

}

// Função : Integrate(A,k).
// Essa função aceita o vetor A de pontos (x,f(x)) com k pontos e
// e subdivide o vetor A em intervalos, de maneira que esses sejam aproximados
// por 1/3 de Simpsom e/ou 3/8 de Simpsom.


float Integrate(point A[],int k)
{
 int u,v;   // Indíces auxiliares que delimitam o intervalo a ser integrado.
 
 float aux; // Variável auxiliar
 
 u = 0; // Indíce base é o primeiro ponto do intervalo.
 v = k; // Indice limite é o tamanho do intervalo.
 
 aux = 0.0; // Inicialização do contador.
 
 if( (k + 1)%2 == 0){
    
    // O intervalo tem um número par de pontos de integração.
    // Utilizamos primeiramente 3/8 de Simpson nos primeiros 4 pontos
 
    aux += Simpson38(A,u);
    u +=3;
    
    // e após, 1/3 de Simpson a cada três pontos do restante do intervalo.
    
    while(u != k){
      
      aux += Simpson13(A,u);
      u +=2;
    }
      
 }
 else{
    
    // O intervalo tem um número ímpar de pontos de integração.
    // Utilizamos 1/3 de Simpson a cada três pontos do intervalo.
    
    while(u != k){
      
      aux += Simpson13(A,u);
      u +=2;
    }   
 
 } 
 
 return(aux);
 
} // end Integrate


int main(void){
 
 // Declarações
 
 point F[MAX]; // Armazena os valores referentes à integral externa 
 point G[MAX]; // Armazena os valores referentes à integral interna
 int m,n;      // Definem a região de integração 
 int i,j;      // Contadores auxiliares
 float result; // Resultado da integral aproximada
 float a,b,c;  // Variáveis auxiliares
 
 // Leitura dos parâmetros iniciais
 
 scanf("%d %d",&m,&n);
  
 if(m < 0 || n < 0) 
  // Volume = ZERO!!!
  printf("0.000000\n");
 
 else{
  
  for(i=0; i<=m; i++){

   for(j=0; j<=n; j++){
     
     // Leitura dos valores referentes a (x,y,f(x,y))
     
     scanf("%f %f %f",&a,&b,&c);     
     F[i].x = a;
     G[j].x = b;
     G[j].fx = c;     
   }   
   
   // Nesse ponto, com x fixado, integramos em y (integral interna)
   // usando a integração numérica. Esse cálculo determina o par (x,f(x,y)).
   
   F[i].fx = Integrate(G,n);  
  }
 
  // Nesse ponto, todos os pares (x,f(x,y)) estão determinados.
  // Integramos em x (integral externa) para obter a aproximação da integral
  // dupla na região dada. 
 
  result = Integrate(F,m);
 
  //Impressão do resultado.
 
  printf("%.6f\n",result);
 
 } 
 return 0;

}

/*****************************************************************************/
/**                     TRABALHO 1 - Cálculo Numérico II                    **/
/*****************************************************************************/

// ALUNO : Ubiratan Soares(5634292) e Thiago Machado(2373831)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
               float x;
               float y;
               }point;

int i,n;              // Contadores auxiliares

float aux1,aux2;      // Variáveis auxiliares(E/S e operações)

float alpha,tetha;    // Variáveis auxiliares(rotação)

float a1,a2,a3,b1,b2; // Variáveis auxiliares(polinômio de saída)

float p0,p1,p2;       // Coeficientes do polinômio de saída

point cloud[50],CM;   // Nuvem de pontos

int main(void)
{
 // Lendo o número de pontos que serão computados;
 
 scanf("%d",&n);
 
 // Lendo a nuvem de pontos e preenchendo o vetor de entrada
 
 for(i=0; i<n; i++){
   scanf("%f %f",&aux1,&aux2);
   cloud[i].x = aux1;
   cloud[i].y = aux2;
 }
 
 // Calculando o centro de massa do conjunto de pontos
 
 CM.x = 0.0;
 CM.y = 0.0;
 
 for(i=0; i<n; i++){
   CM.x += (cloud[i].x/n);
   CM.y += (cloud[i].y/n);
 }
 
 // Transladando os pontos para o sistema que tem como origem
 // o centro de massa
 
 for(i=0; i<n; i++){
   cloud[i].x -= CM.x;
   cloud[i].y -= CM.y;
 } 
 
 // Uma vez que CM é a nova origem do sistema, podemos simplificar 
 // os cálculos para a reta que melhor aproxima a nuvem de pontos pelo MMQ
 
 // Tem-se que o termo independente dessa reta é nulo, bem como as somatórias
 // dos pontos x e y(isso pode ser visto escrevendo-se a fórmula do centro de
 // massa e impondo cmx = 0 = cmy.
 
 // Calculando o coeficiente linear da reta de aproximação
 
 aux1 = 0.0;
 
 aux2 = 0.0;
 
 for(i=0; i<n; i++){
  
  aux1 += (cloud[i].x)*(cloud[i].y);
  
  aux2 += pow(cloud[i].x,2); 
 }
 
 alpha = aux1/aux2; //Alpha é o coeficiente linar de p1~f
 
 // Para rotacionar o sistema, rotacionamos todos os pontos em relação
 // ao ângulo definido por Alpha
 
 tetha = atan(alpha);
 
 // Rotacionamos cada ponto considerando a orientação positiva da nova base 
 // e obtemos uma nuvem de pontos
 
 for(i=0; i<n; i++){
 
 aux1 = cos(tetha)*cloud[i].x + sin(tetha)*cloud[i].y;
 aux2 = cos(tetha)*cloud[i].y - sin(tetha)*cloud[i].x;
 
 cloud[i].x = aux1;
 cloud[i].y = aux2;
 
 }
 
 // Usando os resultados aplicados para calcular a primeira aproximação da reta
 // relacionados ao centro, podemos simplificar os cálculos da parábola apro-
 // ximante. As variáveis abaixo são alguns coeficientes da matriz que define
 // o SL associado ao cálculo da aproximação p2(x)~f.

 a1 = a2 = a3 = b1 = b2 = 0.0;
 
 for(i=0; i<n; i++){
   
   a1 += pow(cloud[i].x,2);
 
   a2 += pow(cloud[i].x,3);
 
   a3 += pow(cloud[i].x,4);
 
   b1 += (cloud[i].x)*(cloud[i].y);
 
   b2 += (pow(cloud[i].x,2))*(cloud[i].y);
 }

 
 // Um estudo prévio do problema no papel leva a várias simplificações,
 // que resultam no seguinte cálculo direto dos coeficientes de p2(x), sendo
 // p0 o termo independente.
 
 p2 = n*(b2*a1 - a2*b1)/(n*a1*a3 - n*pow(a2,2) - pow(a1,3));
 
 p0 = -(a1/n)*p2;
 
 p1 = (1/a1)*(b1-a2*p2);
 
 // Imprimindo o resultado;
 
 printf("%.3f %.3f %.3f\n",p0,p1,p2);

 getchar();getchar();
 
return 0;
}

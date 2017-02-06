// TRABALHO 4 - CÁLCULO NUMÉRICO II
// Trabalho desenvolvido pelos alunos:
// Ubiratan Soares(5634292)
// Thiago Fontes Machado(2373831)

#include <stdio.h>
#include <math.h>

// Função : Derivate(params,G,m1,m2,aux)
// Essa função aceita uma lista de parâmetros de posição e velocidade dos
// corpos 1 e 2, além das massas m1,m2 e a constante G e retorna as derivadas 
// para serem utilizadas no PVI na variável aux.

// DESIGNAÇÃO DOS PARÂMETROS(ENTRADA) :

// params[0] : x1 (Coordenada X de posição do Corpo 1)
// params[1] : y1 (Coordenada Y de posição do Corpo 1)
// params[2] : u1 (Coordenada Inicial de Velocidade do Corpo 1)
// params[3] : v1 (Coordenada Final de Velocidade do Corpo 1)
// params[4] : x2 (Coordenada X de posição do Corpo 2)
// params[5] : y2 (Coordenada Y de posição do Corpo 2)
// params[6] : u2 (Coordenada Inicial de Velocidade do Corpo 2)
// params[7] : v2 (Coordenada Final de Velocidade do Corpo 2)

// DESIGNAÇÃO DOS PARÂMETROS(SAÍDA) :
// aux[0] : x1'
// aux[1] : y1'
// aux[2] : u1'
// aux[3] : v1'
// aux[4] : x2'
// aux[5] : y2'
// aux[6] : u2'
// aux[7] : v2'


void Derivate(double params[],double G,double m1, double m2, double aux[])
{
 // Variáveis auxiliares

 double dx = params[0] - params [4]; // x1-x2 = -(x2-x1)
 double dy = params[1] - params [5]; // y2-y1 = -(y1-y2)

 double norm = sqrt(dx*dx + dy*dy); 
/* OBS :: ||r1 - r2|| = ||r2 - r1|| */ 

 // x1' = u1
 aux[0] = params[2];

 // y1' = v1
 aux[1] = params[3]; 

 // u1' = -G * m2(x1-x2) / (||r1 - r2||^3)
 aux[2] = - G *m2 *dx/(norm * norm * norm);

 // v1' = -G * m2(y1-y2) / (||r1 - r2||^3)
 aux[3] = -G * m2 *dy/(norm * norm * norm);

 // x2' = u2
 aux[4] = params[6];

 // y2' = v2
 aux[5] = params[7];

 // u2' = -G * m1(x2-x1) / (||r2 - r1||^3)
 aux[6] =  G * m1 * dx/(norm * norm * norm);

 // v2' = -G * m1(y2-y1) / (||r2 - r1||^3)
 aux[7] =  G * m1 * dy/(norm * norm * norm);

}

int main(void)
{

 int n;		   // Número de interações pelo qual será aproximada a solução
 double h; 	   // Passo para cada iteração
 double G;         // Constante Gravitacional
 double m1,m2;	   // Massas dos corpos M1 e M2
 double params[8]; // Vetor de condições iniciais do PVI

 // Leitura dos número de iterações
 scanf("%d",&n);

 // Leitura do passo 
 scanf("%lf",&h);

 // Leitura da Constante Gravitacional
 scanf("%lf",&G);

 // Leitura das massas M1 e M2
 scanf("%lf %lf",&m1,&m2);

 // Leituras das coordenadas de posição e velocidade

 // Leitura de x1 e y1
 scanf("%lf %lf",&params[0],&params[1]);

 // Leitura de x2 e y2
 scanf("%lf %lf",&params[4],&params[5]);

 // Leitura de u1 e v1
 scanf("%lf %lf",&params[2],&params[3]);

 // Leitura de u2 e v2
 scanf("%lf %lf",&params[6],&params[7]);

 // Aproximação da solução do sistema de equações através da forma vetorial
 // do método de Runge-Kutta de Ordem 4 em n iterações.

 int i,j;

 double k1[8]; // Vetor k1 para forma vetorial de Runge-Kutta
 double k2[8]; // Vetor k2 para forma vetorial de Runge-Kutta
 double k3[8]; // Vetor k3 para forma vetorial de Runge-Kutta
 double k4[8]; // Vetor k4 para forma vetorial de Runge-Kutta

 double temp[8];// Vetor auxiliar para determinação dos K's



 for(i=0; i<n ; i++){	

	// Determinando o vetor de constantes K1
	 Derivate(params,G,m1,m2,k1);


	// Determinando o vetor auxiliar de constantes
	for(j=0;j < 8;j++)
		temp[j] = params[j] + (h/2.0) * k1[j];

	// Determinando o vetor de constantes K2
	Derivate(temp,G,m1,m2,k2);

	// Reajustando os valores do vetor auxiliar de constantes
	for(j=0;j < 8;j++)
		temp[j] = params[j] + (h/2.0)*k2[j];

	// Determinando o vetor de constantes K3
	Derivate(temp,G,m1,m2,k3);


	// Reajustando novamente os valores do vetor auxiliar de constantes
	for(j=0;j < 8;j++)
		temp[j] = params[j] + h*k3[j];

	// Determinando finalmente o vetor de constantes K4
	Derivate(temp,G,m1,m2,k4);

	// Determinando a aproximação 

	for(j=0; j < 8; j++)
		params[j] += (h/6.0) * (k1[j] + 2*(k2[j] + k3[j]) + k4[j]);

 }// end Runge- Kutta


 // Impressão dos Resultados

 // Impressão de x1 e y1
 printf("%.6lf %.6lf\n",params[0],params[1]);

 // Impressão de x2 e y2
 printf("%.6lf %.6lf\n",params[4],params[5]);

 // Impressão de u1 e v1
 printf("%.6lf %.6lf\n",params[2],params[3]);

 // Impressão de u2 e v2
 printf("%.6lf %.6lf",params[6],params[7]);


 return(0);
}


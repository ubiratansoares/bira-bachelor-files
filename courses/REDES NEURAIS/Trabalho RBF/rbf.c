// Arquivo : rbf.c
// Esse programa ilustra a execução de uma rede RBF, que toma como centros das funções radiais
// os pontos equiespaçados do domínio de entrada segundo o número de neurônios e calcula o vetor
// de pesos com menor erro quadrático possível para classificar os padrões de entrada 

// Trabalho desenvolvido pelos alunos:
// Ubiratan Soares
// Paulo Diniz

// Data : 06/11/2009

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define MAXPATTERNS 50

// Definição do tipo de dados que acomoda os limites superior e inferior de uma dimensão de entrada

typedef struct{
	
	float min;
	float max;
				}range;




float find_centers(float *c,int n,range domain, int m);
void fill_sigmas(float *sigma,float desloc, int n);
void do_rbf(float *H[MAXPATTERNS],int i, int j, float *c, float *sigma,float *x, int n);


// Programa Principal

int main(int argc, char **argv){
	
	/*********************          Variáveis Utilizadas        ***********************/

	int n;	// Dimensão do espaço de entrada
	int m;	// Quantidade de funções radiais (neurônios) utilizados na rede
	
	int patterns;	// Quantidade de padrões (vetores) de entrada a serem classificados
	range* domain;	// Vetor de escopo para cada dimensão do espaço de entrada
	
	int i,j;	// Contadores auxiliares
	float** x;	// Vetores de entrada indexados
	float* y;	// Vetor de saída esperada para cada respectivo vetor de entrada
	float desloc;
	
	float **c;	// Vetores indexados de centros das funções radiais
	float **sigma;	// Vetores de abertura das funções radiais indexados
	float **H;	// Matriz de padrões processados por funções radiais
	
	/*********************    Leitura dos dados de entrada    *************************/
	
	// Leitura da dimensão do espaço de entrada
	
	scanf("%d",&n);
	printf("%d\n",n); 
		
	// Alocação dinâmica de vetor de escopo
	
	domain = (range*) malloc(sizeof(range) * n);
	
	// Leitura dos limites para cada dimensão da entrada
	
	for(i = 0;i < n;i++){
		scanf("%f",&domain[i].min);
		scanf("%f",&domain[i].max);
	}
	
	
	for(i = 0;i < n;i++){
		printf("%f ",domain[i].min);
		printf("%f\n",domain[i].max);
	}
	
	
	// Leitura da quantidade de padrões de entrada
	
	scanf("%d",&patterns);
	printf("%d\n",patterns);

	// Alocação dinâmica de matriz de padrões de entrada
	
	x = (float**) malloc(sizeof(float*) * patterns);
	
	for(i=0; i< patterns ; i++)
		x[i] = (float*) malloc(sizeof(float) * n);
	
	// Alocação dinâmica do vetor de saídas esperadas
	
	y = (float*) malloc(sizeof(float) * n);
	
	// Leitura dos padrões de entrada e suas respectivas saídas desejadas
		
	for(i = 0; i < patterns; i++){
		
		for(j = 0; j < n; j++){
			scanf("%f",&x[i][j]);
		}
		scanf("%f",&y[i]);
	}


	for(i = 0; i < patterns; i++){
		
		for(j = 0; j < n; j++){
			printf("%f ",x[i][j]);
		}
		printf("%f\n",y[i]);
	}

	
	// Leitura do número de neurônios a serem utilizados
	
	scanf("%d",&m);
	printf("%d\n",m);

	/*********************    Montagem da Rede Neural RBF    *************************/


	// Construção da Matriz H de funções radiais calculadas para cada padrão de entrada


	H = (float**) malloc(sizeof(float*) * patterns);
	
	for(i = 0; i < m ; i++)
		H[i] = (float*) malloc(sizeof(float) * m);


	for(i = 0; i < m; i++)
		for(j = 0; j < patterns; j++)
			H[i][j] = 0.0;


	for(i = 0; i < m; i++){	
		for(j = 0; j < patterns; j++)
			printf("%f ",H[i][j]);

		printf("\n");
	}


	// Construção dos vetores de centros para cada função radial
	

	c = (float**) malloc(sizeof(float*) * m);
	
	for(i = 0; i < m ; i++)
		c[i] = (float*) malloc(sizeof(float) * m);


	// Construção dos vetor de abertura para cada função radial
	

	sigma = (float**) malloc(sizeof(float*) * m);
	
	for(i = 0; i < m ; i++)
		sigma[i] = (float*) malloc(sizeof(float) * m);



	printf("\n\nPROCESSAMENTO");

	// Encontrando os centros de cada função radial
	
	for(i = 0; i < m; i++){
		desloc = find_centers(c[i],n,domain[i],m);
	}


	printf("\n\nVETORES DE CENTROS\n");

	for(i = 0; i < m; i++){	

		printf("\nV[%d] = ",i);

		for(j = 0; j < n; j++)
			printf("%f ",c[i][j]);

		
	}
	//printf("\ndesloc = %f \n\n",desloc);	


	for(i = 0; i < m; i++)
		fill_sigmas(sigma[i], desloc, n);


	printf("\n\nVETORES DE ABERTURAS\n");


	for(i = 0; i < m; i++){	

		printf("\nV[%d] = ",i);

		for(j = 0; j < n; j++)
			printf("%f ",sigma[i][j]);

	}

	// Processando cada padrão de entrada com funções radiais e gerando matriz H

	printf("\n\nMATRIZ DE PATTERNS PROCESSADOS\n");


	c[0][0] = 0.0;
	c[0][1] = 0.0;
	c[1][0] = 1.0;
	c[1][1] = 1.0;
	
	for(i = 0; i < m; i++)
		for(j = 0; j < patterns; j++)
			
			do_rbf(H,i,j,c[i],sigma[i],x[j],n);
			

		for(i = 0; i < m; i++){	
			for(j = 0; j < patterns; j++)
				printf("%f ",H[i][j]);

			printf("\n");
		}

	// Daqui para frente vem o código do Turista !!!!!!!!!!!!!!!!!!!!!!
		
	
	return EXIT_SUCCESS;
	
}//end main


float find_centers(float *c,int n,range domain, int m){
	
	float desloc;
	int k;
		
	desloc = (domain.max - domain.min)/(m - 1);
	
	c[0] = domain.min;

	for(k = 1; k < n; k++) c[k] = c[k-1] + desloc;
		
	return(desloc);
		
}//end find_centers


void fill_sigmas(float *sigma,float desloc, int n){
		
	int i;
	
	for(i = 0; i < n; i++) sigma[i] = desloc;
	
	
}//end find_sigmas

void do_rbf(float *H[MAXPATTERNS],int i, int j, float *c, float *sigma,float *x, int n){
	
	float aux;
	int k;
	aux = 1.0;
	
	for(k = 0; k < n; k++){
	
	
		printf("x[%d] = %f ; c[%d] = %f ; sigma[%d] = %f\n",k,x[k],k,c[k],k,sigma[k]);
	
		aux *= exp( - (x[k] - c[k])*(x[k] - c[k]) / (sigma[k] * sigma[k]) );
	
		
	
	}
	
	H[i][j] = aux;
	
}//end do_rbf


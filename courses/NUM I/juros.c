#include <stdio.h>
#include <math.h>

int main(int argc, char** argv){


	float v;
	float j;
	float p;
	float pm;

	printf("VALOR = ");scanf("%f",&v);
	printf("JUROS = ");scanf("%f",&j);
	printf("PRAZO = ");scanf("%f",&p);

	pm = v * j / (1 - pow(1.0 + j,-p) );
	
	printf("\nPARCELA = %.4f\n",pm);
	printf("TOTAL A PRAZO = %4.f\n\n",(float)pm * p);

	return 0;
} 

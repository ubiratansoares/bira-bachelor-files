#include <stdio.h>

int fat(int n);

int main(void){


	int k;
	k = 5;
	
	printf("\n\n%d\n\n",fat(k));

	return 0;

}

int fat(int n){

	int i,aux;

	i = n;
	aux = 1;
	
	while(i>0){
	
		aux *= i;
		i--;
	}

	return(aux);
}
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

/********************************************************************************

// Detalhamento das funções utilizadas

pthread_create(

			pthread_t *thread_handle,
			const pthread_attr_t *attribute,
			void* (*thread_function)(void*), 
			void *arg
			);

onde:

"pthread_create" irá criar um fluxo de execução correspondente ao código deter
minado pela função "thread_function". Em caso de sucesso na criação da thread,
um identificador único é retornado em "thread_handle". Essa thread será criada
com os argumentos especificados em "attribute", enquanto o campo "arg" aponta
para os eventuais argumentos que devem ser tomados pela função thread_function. 


pthread_join(

		pthread_t thread;		
		void** ptr 
	);


********************************************************************************/

#define MAXTHREADS 8

void* computePI(void* s);


int totalhits,
	totalmiss,
	hits[MAXTHREADS],
	samplepts,
	smpptspthread,
	numthreads;
	
int main(void){
	
	int i;
	pthread_t tid[MAXTHREADS];
	pthread_attr_t attr;
	double computedPI;
	
	pthread_attr_init(&attr);
	
	pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM);
	
	samplepts = 1000;
	numthreads = 4;
	totalhits = 0;
	
	smpptspthread = samplepts / numthreads;
	
	for(i = 0; i < numthreads; i++){
		
		hits[i] = i;
		pthread_create(&tid[i], &attr, computePI, (void*)hits[i]);
		
	}
	
	for(i = 0; i < numthreads; i++){
		
		pthread_join(tid[i], NULL);
		totalhits += hits;	
	}
	
	computedPI = 4.0 * (double) totalhits / ((double) samplepts);
	
	printf("\n\n POINTS = %d \n COMPUTED PI = %lf \n\n", samplepts,computedPI);
	
	return 0;
}

void* computePI(void *s){
	
	int seed,i, *hitpointer;
		
	double randx,randy;
	
	int localhits;	
	
	
	hitpointer = (int*)s;
	
	seed = *hitpointer;

	localhits = 0;
	
	for(i = 0; i < smpptspthread; i++){
		
		randx = (double) (rand_r(&seed))/(double)((2<<14) - 1);
		randy = (double) (rand_r(&seed))/(double)((2<<14) - 1);

		if( (randx - 0.5) * (randx - 0.5) + (randy - 0.5) * (randy-0.5) < 0.25 )
			localhits++;
			
		seed *=i;	

	}
	
	*hitpointer = localhits;
	pthread_exit(0);
}
// para compilar: gcc prodcons_1_openmp_sem.c -o prodcons_1_openmp_sem -lpthread -fopenmp
// para executar: prodcons_1_openmp_sem
//
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <omp.h>

#define MAX_PRODUCED 30
#define MAX_QUEUE 5

sem_t  empty, full;

int queue[MAX_QUEUE], item, item_available=0, produced=0, consumed=0;


int create_item(void) {

	return(rand()%1000);

} //fim  create_item()

void insert_into_queue(int item) {
	
	queue[item_available++] = item;
	produced++;
	printf("producing item:%d, value:%d, queued:%d \n", produced, item, item_available); 
	return;

} // fim insert_into_queue()

int extract_from_queue() {

	consumed++;
	printf("cosuming item:%d, value:%d, queued:%d \n", consumed, queue[item_available-1], item_available-1); 
	
	return(queue[--item_available]);

} // fim extract_from_queue()

void process_item(int my_item) {
	static int printed=0;

	printf("Printed:%d, value:%d, queued:%d \n", ++printed, my_item, item_available);

	return;

} // fim_process_item()


void *producer(void) {
	int item;

	while (produced < MAX_PRODUCED ) {
		item = create_item();
		
		sem_wait(&empty);
		#pragma omp critical (mutex)
		{
		    insert_into_queue(item);
		}
        	sem_post(&full);
		
		#pragma omp flush
	}

} // fim producer


void *consumer(void) {
	int item = 0;

	while (consumed < MAX_PRODUCED) {
		sem_wait(&full);
		#pragma omp critical (mutex)
		{
		    item = extract_from_queue();
		}
        	sem_post(&empty);
        	process_item(item);

		#pragma omp flush
	
	}
} // fim_consumer


int main(void) {
	pthread_t prod_handle, cons_handle;

	sem_init(&empty, 0, MAX_QUEUE);
	sem_init(&full, 0, 0);

	/* create and join producer and consumer threads */

	#pragma omp parallel sections num_threads(2) private(item) shared(item_available, queue)
	{
	  
	  #pragma omp section
	  {
	      producer();
	  }
	  
	  #pragma omp section
	  {
	      consumer();
	  }
	  
	}
	
	printf("main está saindo... \n");
	fflush(0);

	exit(0);
	
} // main()
